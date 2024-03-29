# PreAmplifier project

This is the C source code for a modular preamplifier, so the features depend a lot on the different pcb's that are used. The heart of the preamp is the PreampController pcb, which connects the other pcb's, display, buttons, encoder, infrared receiver, etc. Other sensible configurations are:

- Standalone stereo or 5.1 input selection (switch inputs using buttons and infrared, no volume control)
- Standalone DAC in software mode (volume is controlled by the DAC)
- Standalone VolumeControl (without input selection, so single input)
- Combinations of the above (e.g. 5.1 input selection + volume control)
 - Stereo input selection + volume control
 - Stereo + 5.1 input selection + volume control
 - Stereo input selection + DAC + volume control
 - Stereo + 5.1 input selection + DAC + volume control
 - DAC + volume control

## Links

[PreAmplifier project page](http://www.djuke.nl/en/projects/10-pre-amplifiers/34-preamplifierv2)

[PreAmplifier products](https://webshop.djuke.nl/index.php?route=product/category&path=60_68)

[PreAmplifier support pages](http://www.djuke.nl/en/support/15-preamplifier)

## Building

### XC8 (from v1.4)
Newer firmware is built using the free Microchip XC8 C compiler (I use v2.40 on Linux). Download and install it from the [XC8 download page](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/downloads-documentation#XC8).

#### Command line
From the project directory, simply run:

```
make
```

If no errors occurred, this will result in `dist/default/production/preamp.production.hex` being generated.

#### MPLAB X IDE
The project can be opened and adjusted using the Microchip MPLAB X IDE.

The following project settings are important:
* XC8 Global Options -> Stack options -> Stack type: `Hybrid`
* XC8 Linker -> Memory model -> ROM ranges: `default, -FF00-FFFF` (bootloader)
* XC8 Linker -> Runtime -> Initialize data
* XC8 Linker -> Runtime -> Format hex file for download

### SDCC (up to v1.3)
Building the software for earlier versions is done in Linux and requires SDCC (small device C Compiler) to be installed. Because the non-free pic16 is typically not included in SDCC from major Linux distributions it is advised to download it from the [SDCC homepage](http://sdcc.sourceforge.net)

From the source code directory, simply run:

```
make
```

If no errors occurred, this will result in `DjukePreampV2.hex` being generated. Temporary files can be removed using:

```
make clean
```

## Changelog

### version 1.4
Release date: 2023-11-07
- Move from SDCC to XC8 with major flash size reduction
- Move DAC handling from interrupt to main loop
- Increase max title size to 80 characters

IMPORTANT: If you use UnixTinyBootLoaderLoader to update the firmware make sure to use at least utbll v1.1 as this fixes issues with hex files consisting of multiple chunks

### version 1.3
Release date: 2021-07-02
- Reduce sensitivity of encoder in HW setup
- Input names in alphabetic order
- Fix title display in menu
- Make (un)signed datatypes explicit to allow compilation with recent SDCC
- Fix mute state during power on

### version 1.2
Release date: 2020-04-13
- Increased speed of title scrolling to about 4 characters/s
- Limited max master 1/2/3 volume to +3dB

### version 1.1
Release date: 2016-12-21
- Python script now python3 compatible
- Added 'spare' to list of inputs
- Defined input names as array to save space

### version 1.0
Release date: 2016-04-18
- Added makefile for compiling from command line
- Added new style configuration bits
- Handle UART at high priority
- Set title only for specific input
- Added python tools for integration with Volumio/Rune
- Improved debug mechanism
- Switch on power to have backlight in hardware setup menu

### version 0.9
Release date: 2016-02-15
- Interrupt based infrared
- Visualisation of input level
- Detection of elapsed time without signal
- Support different hardware setups
- Loading/saving from/to eeprom
- Set RC5 infrared address
- Auto switch off function when silent for a long time
- Uptime, ontime and standby time information
- Generic parameter settings
- Terminal support for controlling preamp from UART
- InputSelect and DAC can be used standalong (without VolumeControl)
- Event based control (buttons, encoder, infrared, etc)
- Added DAC error codes
- Support for (chained) 8channel VolumeControl pcbs (up to 24 channels)
- Display title from UART
- Increased number of DAC inputs to 5 (to be used by Raspberry Pi)


