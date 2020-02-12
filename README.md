# SimpleVirusUSBMidiDriver

A CoreMIDI Driver for the Access Virus TI2.

## Description

This driver enables the MIDI interfaces of an [Access Virus TI2](https://virus.info/).

While the audio section of the synthesizer is class-compliant, the MIDI interface needs
some additional non-standard steps during the device configuration.

This driver does not support any of the "Total Integration" features of the synthesizer,
however it can be used to restore a minimal setup if the official Access drivers cannot
be installed.

## Support

The minimal requirement is macOS 10.8 (Mountain Lion), and it is tested to work on the
current macOS version, 10.15 (Catalina).

Unfortunately, since this driver requires exclusive access to the Virus, it is *not*
possible to run it alongside the official Access drivers.

## Acknowledgements

The driver reuses Apple's
[Sample USB MIDI Driver](https://developer.apple.com/library/archive/samplecode/SampleUSBMIDIDriver/Introduction/Intro.html)
reference implementation.

Additional thanks to Yves Salathe, who fixed Apple's project infrastructure in
[GenericUSBMIDI](https://github.com/ysalathe/GenericUSBMIDI), and Ferland Francois, for
the work on [casiousbmididriver](https://code.google.com/archive/p/casiousbmididriver).
