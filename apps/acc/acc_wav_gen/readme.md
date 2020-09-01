---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title: ACC PWM generation 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# ACC PWM generation

This example uses ACC peripheral Library to generate a PWM waveform with variable duty cycle.

## Description

This application example uses Analog Comparator to generate a PWM waveform by comparing the sine wave generated from the DAC with an external input voltage. Duty cycle of the PWM waveform is inversely proportional to the input voltage.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/acc/acc_wav_gen/firmware** .

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

- Comparator Positive Input: Connect an external voltage source to PD30 (Pin 3 of the J501)
- Comparator Negative Input: Sine wave from the DAC Channel 0. No connection required
- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Comparator Positive Input: Connect an external voltage source to PD30 (Pin 28 of the Camera Interface connector)
- Comparator Negative Input: Sine wave from the DAC Channel 0 (No connection required)
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and program the application project using its respective IDE
2. Observe the generated PWM waveform on the output pin using an oscilloscope
3. Change the positive input of the comparator from 0 to 3.3V and observe the duty cycle of the PWM waveform changing from 0 to 100 percentage

Refer to the table below for the PWM output pin of different boards:

| Board      | PWM output pin                                    |
| ----------------- | ---------------------------------------------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | PB03 (Pin 7 of the J502 Connector) |
| [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)      |PB03 (Pin 20 of the Camera Interface connector) |
|||
