---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title: DACC waveform generation 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# DACC waveform generation

This example shows how to use the DACC peripheral to generate a 5 KHz sine wave or triangle wave using a lookup table.

## Description

The DACC Peripheral library is used to generate either sine wave or triangle wave, by writing digital data to the DACC in periodic callback on every Timer Counter (TC) Interrupt.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/dacc/dacc_wav_gen/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X    | MPLABX Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
| sam_v71_xult.X    | MPLABX Project for  [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
|sam_e70_xult.X | [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|sam_v71_xult.X | [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|
|||

### Setting up [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Connect the DACC "Channel 0" and "Channel 1" to oscilloscope probe for observing the analog output waveform
  - DACC Channel 0 is available through PB13 (Pin 5 of the J502 connector)
  - DACC Channel 1 is available through PD00 (Pin 6 of J502 connector)
- PD00 line is connected to the external oscillator on the board to act as Ethernet Clock and it needs to be disabled to observe the DACC Channel 1 output
  - Short the Jumper (J805) located near the Ethernet PHY interface to Disable the Oscillator
- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Connect the DACC "Channel 0" and "Channel 1" output pin to oscilloscope probe for observing the analog output waveform
  - DACC Channel 0 is available through PB13 (Pin 5 of the J504 connector)
  - DACC Channel 1 is available through PD00 (Pin 6 of J504 connector)
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application using its respective IDE
2. Observe a sine wave of 5 KHz frequency on DACC output pins
3. Press switch SW0 to toggle the waveform shape from sine wave to triangle wave and back

| Board      | DACC output pins |
| ----------------- | ---------------------------------------------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | PB13 (Pin 5 of J502 connector) <br> PD00 (Pin 6 of J502 connector) |
| [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)      | PB13 (Pin 5 of J504 connector) <br> PD00 (Pin 6 of J504 connector)|
|||
