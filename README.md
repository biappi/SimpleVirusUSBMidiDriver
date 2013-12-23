GenericUSBMIDI
==============

This is an OS X driver for MIDI devices connected via USB. It has been tested with a Roland (Ed) PC-300 MIDI Keyboard connected via USB to an Apple MacBook Pro running Mac OS X v10.8.2 (Mountain Lion).

I started this project since at the time of writing, Roland does not provide drivers for the PC-300 that work with OS X v10.8 (Mountain Lion). 

At the present stage it consists of sample code from Apple Inc. which was obtained from the casiousbmididriver project (http://code.google.com/p/casiousbmididriver/).

The only change to the sample code that was necessary to get the driver to work with the Roland PC-300, was to set the USB interface number, vendor ID and product ID in the file GenericUSBMidi.cp:

    #define kTheInterfaceToUse	2	// The third one
    #define kMyVendorID		0x0582	// Roland Corporation
    #define kMyProductID	0x0008

The idea of the present project would be to provide a generic open source OS X driver for several other USB MIDI devices. Contributions are welcome! 


After compiling the source with Xcode simply copy the resulting bundle GenericUSBMidi.plugin to the Folder /Library/Audio/MIDI Drivers/. This is most simply done with the following bash (Terminal) commands:

    cd $HOME/Library/Developer/Xcode/DerivedData/GenericUSBMidi*/Build/Products/Debug
    sudo cp -r GenericUSBMidi.plugin /Library/Audio/MIDI\ Drivers/

Note: to find out where the "GenericUSBMidi.plugin" file has been created, right-click onto Products->GenericUSBMidi.plugin in Xcode and then click "Show in Finder". This is illustrated in the file ShowInFinder.png:

[screenshot to illustrate what to do in Xcode to find the compiled plugin in Finder](ShowInFinder.png)

After that it is necessary to reboot the MIDIServer process (or the entire OS). 

For debugging (e.g. using break points) attach your debugger to the MIDIServer process.
In Xcode this is done via the Menu Product -> Attach to Process -> MIDIServer.

Have fun!

December 30, 2012

Yves Salathe
