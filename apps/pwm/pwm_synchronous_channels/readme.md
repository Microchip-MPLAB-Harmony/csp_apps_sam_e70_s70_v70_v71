---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title: PWM Generation 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# PWM Generation

This example shows how to use the PWM peripheral to generate 3-phase PWM signals with dead time.

## Description

This example shows how to configure the PWM to generate synchronous 3-phase PWM signals with dead time (used for motor control). The duty cycle of the PWM is updated in the interrupt handler.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/pwm/pwm_synchronous_channels/firmware** .

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

- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application using their respective IDEs
2. Observe the high-side and low-side PWM waveforms on the oscilloscope
3. Observe the dead time between the high side and low side

    ![output](images/output_pwm_synchronous_channels.png)

Refer to the below table for PWM output pins for different boards:

| PWM Channel      | [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113) | [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult) |
| ---------|---------------------- | ---------------------|
| CH0_PWMH | PA00 (Pin 7 of EXT1)  | PA00 (Pin 7 of EXT1) |
| CH0_PWML | PA19 (Pin 4 of EXT1)  | PA19 (Pin 4 of EXT1) |
| CH1_PWMH | PA02 (Pin 9 of EXT2)  | PA02 (Pin 9 of EXT2) |
| CH1_PWML | PD25 (Pin 15 of EXT1) | D25 (Pin 15 of EXT1) |
| CH2_PWMH | PC19 (Pin 7 of EXT2)  | PC19 (Pin 7 of EXT2) |
| CH2_PWMH | PD26 (Pin 8 of EXT2)  | PD26 (Pin 8 of EXT2) |
