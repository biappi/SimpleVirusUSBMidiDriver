# SimpleVirusUSBMidiDriver

A CoreMIDI Driver for the Access Virus TI/TI2.

## Description

This driver enables the MIDI interfaces of an [Access Virus TI/TI2](https://virus.info/).

While the audio section of the synthesizer is class-compliant, the MIDI interface needs
some additional non-standard steps during the device configuration.

This driver does not support any of the "Total Integration" features of the synthesizer,
however it can be used to restore a minimal setup if the official Access drivers cannot
be installed.

## Support

The minimal requirement is macOS 10.8 (Mountain Lion), and it is tested to work on the
current macOS version, 12.1 (Monterey).

Unfortunately, since this driver requires exclusive access to the Virus, it is *not*
possible to run it alongside the official Access drivers.


## Installation

### Prerequisites

Installing the driver from source requires [Xcode](https://developer.apple.com/xcode/)
that can be downloaded at it's [Mac App Store page](https://apps.apple.com/us/app/xcode/id497799835?mt=12).

Download the source by either cloning this repository, or downloading it as
a [zip file](https://github.com/biappi/SimpleVirusUSBMidiDriver/archive/refs/heads/master.zip).


### Alternative 1 - Using the Terminal

Navigate to the directory where you downloaded the sources

    willy@Thala Sources$ cd SimpleVirusUSBMidiDriver/
    willy@Thala SimpleVirusUSBMidiDriver master$ 

Build the product

    willy@Thala SimpleVirusUSBMidiDriver master$ xcodebuild 

        [ ... lots of compilation output ... ]

    ** BUILD SUCCEEDED **

    willy@Thala SimpleVirusUSBMidiDriver master$

Then, copy the driver in the system location, using superuser permissions

    willy@Thala SimpleVirusUSBMidiDriver master$ sudo cp -r build/Release/SimpleVirusUSBMidi.plugin /Library/Audio/MIDI\ Drivers/SimpleVirusUSBMidi.plugin
    Password:
    willy@Thala SimpleVirusUSBMidiDriver master$ 


### Alternative 2 - Using the GUI

Open the project by double clicking the `SimpleVirusUSBMidi.xcodeproj` project file.

Build the project by clicking the "play" button. After completion, you can find the file in
the `Products` folder.

You can drag and drop the product from the Xcode window to the `/Library/Audio/MIDI Drivers/`
folder.

### Figure 1 - Xcode compilation output

![Xcode Screenshot](https://github.com/biappi/SimpleVirusUSBMidiDriver/blob/images/where-to-find-product.png?raw=true)

## Acknowledgements

The driver reuses Apple's
[Sample USB MIDI Driver](https://developer.apple.com/library/archive/samplecode/SampleUSBMIDIDriver/Introduction/Intro.html)
reference implementation.

Additional thanks to Yves Salathe, who fixed Apple's project infrastructure in
[GenericUSBMIDI](https://github.com/ysalathe/GenericUSBMIDI), and Ferland Francois, for
the work on [casiousbmididriver](https://code.google.com/archive/p/casiousbmididriver).
