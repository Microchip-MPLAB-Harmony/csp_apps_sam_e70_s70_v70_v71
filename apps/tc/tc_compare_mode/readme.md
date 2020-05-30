[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Timer Counter (TC) compare mode example (tc_compare_mode)

This example demonstrates how to use the TC module in compare mode to generate an active low, active high, and toggle output on compare match.

## Description

Three TC channels are configured in compare mode. Each channel generates different output waveform depending upon configured action on compare match and period match.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/tc/tc_compare_mode/firmware** .

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

- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application using its respective IDE
2. Observe generated waveforms on the oscilloscope

### [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

| Timer Channel   | Pin      | Observable characteristic of the waveform
| ----------------| ---------| -----------------------------------------|
| TC0_TIOA0 | PA00(Pin 7 of EXT1) | Toggle output with 50Hz frequency |
| TC3_TIOA11 | PD21(Pin 16 of EXT1)  | Active low output with 100 Hz frequency |
| TC3_TIOB12 | PD22 (Pin 18 of EXT1) | Active high output with 100 Hz frequency |

### [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

| Timer Channel   | Pin      | Observable characteristic of the waveform
| ----------------| ---------| -----------------------------------------|
| TC0_TIOA0 | PA00(Pin 7 of EXT1) | Toggle output with 50Hz frequency |
| TC3_TIOA11 | PD21(Pin 16 of EXT1)  | Active low output with 100 Hz frequency |
| TC3_TIOB12 | PD22 (Pin 18 of EXT1) | Active high output with 100 Hz frequency |