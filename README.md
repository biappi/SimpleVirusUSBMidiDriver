GenericUSBMIDI
==============

This is an OS X driver for MIDI devices connected via USB. It has been tested with a Roland PC-300 MIDI Keyboard under Mac OS 10.8.2

After compiling the source with Xcode simply copy the resulting bundle GenericUSBMidi.plugin to the Folder /Library/Audio/MIDI Drivers/. This is most simply done with the following bash (Terminal) command:

sudo cp -r GenericUSBMidi.plugin /Library/Audio/MIDI\ Drivers/

