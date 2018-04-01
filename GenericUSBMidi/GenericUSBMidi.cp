/*	Copyright � 2007 Apple Inc. All Rights Reserved.
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by
 Apple Inc. ("Apple") in consideration of your agreement to the
 following terms, and your use, installation, modification or
 redistribution of this Apple software constitutes acceptance of these
 terms.  If you do not agree with these terms, please do not use,
 install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc.
 may be used to endorse or promote products derived from the Apple
 Software without specific prior written permission from Apple.  Except
 as expressly stated in this notice, no other rights or licenses, express
 or implied, are granted by Apple herein, including but not limited to
 any patent rights that may be infringed by your derivative works or by
 other works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#include "GenericUSBMidi.h"


// ^^^^^^^^^^^^^^^^^^ THINGS YOU MUST CUSTOMIZE ^^^^^^^^^^^^^^^^^^^^^^

// Generated UUID: 204C5239-F14F-440B-A743-4F78DF2BEE26
#define kFactoryUUID CFUUIDGetConstantUUIDWithBytes(NULL, 0x20, 0x4C, 0x52, 0x39, 0xF1, 0x4F, 0x44, 0x0B, 0xA7, 0x43, 0x4F, 0x78, 0xDF, 0x2B, 0xEE, 0x26)

#define kTheInterfaceToUse	2		// The third one

#define kMyVendorID			0x0582	// Roland Corporation
#define kMyProductID		0x0008
#define kMyNumPorts			1		// One in-out pair.

// __________________________________________________________________________________________________


// Implementation of the factory function for this type.
extern "C" void *NewGenericUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID);
extern "C" void *NewGenericUSBMidiDriver(CFAllocatorRef allocator, CFUUIDRef typeID)
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
	int n =
    USBMIDIPrepareOutput(usbmDev, writeQueue, destBuf, usbmDev->mOutPipe.mMaxPacketSize);
	if (n < usbmDev->mOutPipe.mMaxPacketSize) {
		memset(destBuf + n, 0, usbmDev->mOutPipe.mMaxPacketSize - n);
	}
	return usbmDev->mOutPipe.mMaxPacketSize;
}
