#include "USBVendorMIDIDriver.h"

// - //

extern "C" void *NewSimpleVirusUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID);

// - //

#define kMyVendorID         0x0582
#define kMyProductID        0x0008
#define kTheInterfaceToUse  2
#define kMyNumPorts         1

// Factory UUID: A1F2F3CB-5A1F-4066-9255-96557C3DD539
#define kFactoryUUID CFUUIDGetConstantUUIDWithBytes(\
    NULL, \
    0xA1, \
    0xF2, \
    0xF3, \
    0xCB, \
    0x5A, \
    0x1F, \
    0x40, \
    0x66, \
    0x92, \
    0x55, \
    0x96, \
    0x55, \
    0x7C, \
    0x3D, \
    0xD5, \
    0x39  \
)

// - //

class SimpleVirusUSBMidiDriver : public USBVendorMIDIDriver {
public:
    
    SimpleVirusUSBMidiDriver()
        : USBVendorMIDIDriver(kFactoryUUID)
    {
    }

    ~SimpleVirusUSBMidiDriver()
    {
    }

    // USBMIDIDriverBase overrides
    
    virtual bool MatchDevice(USBDevice * inUSBDevice)
    {
        const IOUSBDeviceDescriptor * devDesc = inUSBDevice->GetDeviceDescriptor();
        
        return ((USBToHostWord(devDesc->idVendor)  == kMyVendorID) &&
                (USBToHostWord(devDesc->idProduct) == kMyProductID));
    }
    
    virtual MIDIDeviceRef CreateDevice(USBDevice *    inUSBDevice,
                                       USBInterface * inUSBInterface)
    {
        MIDIDeviceRef dev;
        MIDIEntityRef ent;
        
        CFStringRef boxName = CFSTR("Roland PC-300 USB Keyboard");
        
        MIDIDeviceCreate(Self(),
                         boxName,
                         CFSTR("Roland"),
                         boxName,
                         &dev);
        
        // make entity for each port, with 1 source, 1 destination
        for (int port = 1; port <= kMyNumPorts; ++port) {
            char portname[64];
            
            if (kMyNumPorts > 1)
                sprintf(portname, "Port %d", port);
            else
                CFStringGetCString(boxName, portname, sizeof(portname), kCFStringEncodingMacRoman);
            
            CFStringRef str = CFStringCreateWithCString(NULL, portname, 0);
            MIDIDeviceAddEntity(dev, str, false, 1, 1, &ent);
            CFRelease(str);
        }
        
        return dev;
    }

    virtual USBInterface * CreateInterface(USBMIDIDevice * device)
    {
        USBInterface *intf = device->mUSBDevice->FindInterface(kTheInterfaceToUse, 0);
        return intf;
    }

    // pipes are opened, do any extra initialization (send config msgs etc)
    virtual void StartInterface(USBMIDIDevice * usbmDev)
    {

    }
    
    // pipes are about to be closed, do any preliminary cleanup
    virtual void StopInterface(USBMIDIDevice * usbmDev)
    {
        
    }
    
    // a USB message arrived, parse it into a MIDIPacketList and call MIDIReceived
    virtual void HandleInput(USBMIDIDevice * usbmDev,
                             MIDITimeStamp   when,
                             Byte *          readBuf,
                             ByteCount       readBufSize)
    {
        USBMIDIHandleInput(usbmDev, when, readBuf, readBufSize);
    }

    // dequeue from WriteQueue into a single USB message, return
    virtual ByteCount PrepareOutput(USBMIDIDevice * usbmDev,
                                     WriteQueue &   writeQueue,
                                     Byte *         destBuf)
    {
        ByteCount n =
            USBMIDIPrepareOutput(usbmDev, writeQueue, destBuf, usbmDev->mOutPipe.mMaxPacketSize);
        
        if (n < usbmDev->mOutPipe.mMaxPacketSize) {
            memset(destBuf + n, 0, usbmDev->mOutPipe.mMaxPacketSize - n);
        }
        
        return usbmDev->mOutPipe.mMaxPacketSize;
    }
    
};

// - //

extern "C" void *NewSimpleVirusUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID)
{
	// If correct type is being requested, allocate an
	// instance of TestType and return the IUnknown interface.
    
	if (CFEqual(typeID, kMIDIDriverTypeID)) {
		SimpleVirusUSBMidiDriver *result = new SimpleVirusUSBMidiDriver;
		return result->Self();
	}
    else {
		// If the requested type is incorrect, return NULL.
		return NULL;
	}
}
