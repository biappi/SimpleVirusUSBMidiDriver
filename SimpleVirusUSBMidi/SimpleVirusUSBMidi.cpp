#include "USBVendorMIDIDriver.h"

// - //

extern "C" void *NewSimpleVirusUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID);

// - //

const uint16_t kUSBVendorIdKemper  = 0x133e;
const uint16_t kUSBProductIdVirus  = 0x0815;

const int      kUSBMidiInterface   = 3;

static CFUUIDRef DriverFactoryUUID() {
    // Factory UUID: A1F2F3CB-5A1F-4066-9255-96557C3DD539
    
    return CFUUIDGetConstantUUIDWithBytes(
      NULL,
      0xA1,
      0xF2,
      0xF3,
      0xCB,
      0x5A,
      0x1F,
      0x40,
      0x66,
      0x92,
      0x55,
      0x96,
      0x55,
      0x7C,
      0x3D,
      0xD5,
      0x39
  );
}

// - //

class SimpleVirusUSBMidiDriver : public USBVendorMIDIDriver {
public:
    
    SimpleVirusUSBMidiDriver()
        : USBVendorMIDIDriver(DriverFactoryUUID())
    {
    }

    ~SimpleVirusUSBMidiDriver()
    {
    }

    // USBMIDIDriverBase overrides
    
    virtual bool MatchDevice(USBDevice * inUSBDevice)
    {
        const IOUSBDeviceDescriptor * devDesc = inUSBDevice->GetDeviceDescriptor();
        
        return ((USBToHostWord(devDesc->idVendor)  == kUSBVendorIdKemper) &&
                (USBToHostWord(devDesc->idProduct) == kUSBProductIdVirus));
    }
    
    virtual MIDIDeviceRef CreateDevice(USBDevice *    inUSBDevice,
                                       USBInterface * inUSBInterface)
    {
        MIDIDeviceRef dev;
        MIDIEntityRef ent;
        
        MIDIDeviceCreate(Self(), CFSTR("Simple Virus USB"), CFSTR("Access"), CFSTR("Virus"), &dev);
        MIDIDeviceAddEntity(dev, CFSTR("Virus External Midi"), false, 1, 1, &ent);
        MIDIDeviceAddEntity(dev, CFSTR("Virus Internal Synth"), false, 1, 1, &ent);
        
        return dev;
    }

    virtual USBInterface * CreateInterface(USBMIDIDevice * device)
    {
        return device->mUSBDevice->FindInterface(kUSBMidiInterface, 0);
    }

    // pipes are opened, do any extra initialization (send config msgs etc)
    virtual void StartInterface(USBMIDIDevice * usbmDev)
    {
        uint8_t buf[] = { 0x4e, 0x73, 0x52, 0x01 };
        
        auto usbif = *(usbmDev->mUSBIntfIntf);
        
        usbif->WritePipe(
            usbmDev->mUSBIntfIntf,
            usbmDev->mOutPipe.mPipeIndex,
            buf,
            sizeof(buf)
        );
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
