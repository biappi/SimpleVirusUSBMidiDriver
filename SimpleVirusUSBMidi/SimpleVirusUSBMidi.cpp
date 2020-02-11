#include "USBVendorMIDIDriver.h"


// ^^^^^^^^^^^^^^^^^^ THINGS YOU MUST CUSTOMIZE ^^^^^^^^^^^^^^^^^^^^^^

// Generated UUID: A1F2F3CB-5A1F-4066-9255-96557C3DD539

#define kFactoryUUID CFUUIDGetConstantUUIDWithBytes(NULL, 0xA1, 0xF2, 0xF3, 0xCB, 0x5A, 0x1F, 0x40, 0x66, 0x92, 0x55, 0x96, 0x55, 0x7C, 0x3D, 0xD5, 0x39)

#define kTheInterfaceToUse	2		// The third one

#define kMyVendorID			0x0582	// Roland Corporation
#define kMyProductID		0x0008
#define kMyNumPorts			1		// One in-out pair.

// __________________________________________________________________________________________________

class GenericUSBMidiDriver : public USBVendorMIDIDriver {
public:
    GenericUSBMidiDriver();
    ~GenericUSBMidiDriver();
    
    // USBMIDIDriverBase overrides
    virtual bool            MatchDevice(        USBDevice *        inUSBDevice);
    
    virtual MIDIDeviceRef    CreateDevice(        USBDevice *        inUSBDevice,
                                         USBInterface *    inUSBInterface);
    
    virtual USBInterface *    CreateInterface(    USBMIDIDevice *    usbmDev);
    
    virtual void        StartInterface(            USBMIDIDevice *    usbmDev);
    // pipes are opened, do any extra initialization (send config msgs etc)
    
    virtual void        StopInterface(            USBMIDIDevice *    usbmDev);
    // pipes are about to be closed, do any preliminary cleanup
    
    virtual void        HandleInput(            USBMIDIDevice *    usbmDev,
                                    MIDITimeStamp    when,
                                    Byte *            readBuf,
                                    ByteCount         readBufSize);
    // a USB message arrived, parse it into a MIDIPacketList and
    // call MIDIReceived
    
    virtual ByteCount    PrepareOutput(            USBMIDIDevice *    usbmDev,
                                      WriteQueue &    writeQueue,
                                      Byte *            destBuf);
    // dequeue from WriteQueue into a single USB message, return
};

// __________________________________________________________________________________________________

// Implementation of the factory function for this type.
extern "C" void *NewSimpleVirusUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID);
extern "C" void *NewSimpleVirusUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID)
{
	// If correct type is being requested, allocate an
	// instance of TestType and return the IUnknown interface.
	if (CFEqual(typeID, kMIDIDriverTypeID)) {
		GenericUSBMidiDriver *result = new GenericUSBMidiDriver;
		return result->Self();
	} else {
		// If the requested type is incorrect, return NULL.
		return NULL;
	}
}

// __________________________________________________________________________________________________

GenericUSBMidiDriver::GenericUSBMidiDriver() :
USBVendorMIDIDriver(kFactoryUUID)
{
}

GenericUSBMidiDriver::~GenericUSBMidiDriver()
{
}

// __________________________________________________________________________________________________

bool		GenericUSBMidiDriver::MatchDevice(	USBDevice *		inUSBDevice)
{
	const IOUSBDeviceDescriptor * devDesc = inUSBDevice->GetDeviceDescriptor();
	if (USBToHostWord(devDesc->idVendor) == kMyVendorID) {
		UInt16 devProduct = USBToHostWord(devDesc->idProduct);
		if (devProduct == kMyProductID)
			return true;
	}
	return false;
}

MIDIDeviceRef	GenericUSBMidiDriver::CreateDevice(	USBDevice *		inUSBDevice,
                                                 USBInterface *	inUSBInterface)
{
	MIDIDeviceRef dev;
	MIDIEntityRef ent;
	//UInt16 devProduct = USBToHostWord(inUSBDevice->GetDeviceDescriptor()->idProduct);
	
	CFStringRef boxName = CFSTR("Roland PC-300 USB Keyboard");
	MIDIDeviceCreate(Self(),
                     boxName,
                     CFSTR("Roland"),	// manufacturer name
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

USBInterface *	GenericUSBMidiDriver::CreateInterface(USBMIDIDevice *device)
{
	USBInterface *intf = device->mUSBDevice->FindInterface(kTheInterfaceToUse, 0);
	return intf;
}

void		GenericUSBMidiDriver::StartInterface(USBMIDIDevice *usbmDev)
{
}

void		GenericUSBMidiDriver::StopInterface(USBMIDIDevice *usbmDev)
{
}

void		GenericUSBMidiDriver::HandleInput(USBMIDIDevice *usbmDev, MIDITimeStamp when, Byte *readBuf, ByteCount readBufSize)
{
	USBMIDIHandleInput(usbmDev, when, readBuf, readBufSize);
}

ByteCount	GenericUSBMidiDriver::PrepareOutput(USBMIDIDevice *usbmDev, WriteQueue &writeQueue,
                                              Byte *destBuf)
{
	ByteCount n =
    USBMIDIPrepareOutput(usbmDev, writeQueue, destBuf, usbmDev->mOutPipe.mMaxPacketSize);
	if (n < usbmDev->mOutPipe.mMaxPacketSize) {
		memset(destBuf + n, 0, usbmDev->mOutPipe.mMaxPacketSize - n);
	}
	return usbmDev->mOutPipe.mMaxPacketSize;
}
