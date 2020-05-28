[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Analog Comparator Controller PWM Example (acc_wave_gen)

## Description

This application example uses Analog Comparator to generate a PWM waveform by comparing the sine wave generated from the DAC with an external input voltage. Duty cycle of the PWM waveform is inversely proportional to the input voltage.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/acc/acc_wav_gen/firmware** .

To build the application, refer to the following table and open the project in their respective IDEs.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X    | MPLABX Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
| sam_v71_xult.X    | MPLABX Project for  [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|        |

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
|sam_e70_xult.X|[![sam_e70_xult](https://www.microchip.com/_ImagedCopy/180730-MCU32-PHOTO-DM320113-Angle-7x5.jpg)](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|sam_v71_xult.X|[![sam_v71_xult](https://www.microchip.com/_ImagedCopy/ATSAMV71-XULT_angle.jpg)](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|

### Setting up [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Comparator Positive Input: Connect an external voltage source to PD30 (Pin 3 of the J501)
- Comparator Negative Input: Sine wave from the DAC Channel 0. No connection required
- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Comparator Positive Input: Connect an external voltage source to PD30 (Pin 28 of the Camera Interface connector)
- Comparator Negative Input: Sine wave from the DAC Channel 0. No connection required
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application project using their respective IDEs
2. Observe the generated PWM waveform on the output pin using an oscilloscope
3. Change the positive input of the comparator from 0 to 3.3V and observe the duty cycle of the PWM waveform changing from 0 to 100 percentage

| Board      | PWM output pin                                    |
| ----------------- | ---------------------------------------------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | PB03 (Pin 7 of the J502 Connector) |
| [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)      |PB03 (Pin 20 of the Camera Interface connector) |
