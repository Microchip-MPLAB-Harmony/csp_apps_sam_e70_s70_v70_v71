[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Parallel Input/Output Controller LED on/off using polling example (pio_led_on_off_polling)

This example application demonstrate how to poll the switch input, and indicate the switch status using the LED.

## Description

This application uses the PIO Peripheral library to read GPIO pin connected to the switch, and drives the GPIO pin connected to the LED to indicate switch status. LED is turned ON when the switch is pressed and turned OFF when the switch is released.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/pio/pio_led_on_off_polling/firmware** .

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

1. Build and Program the application using their respective IDEs
2. LED is turned ON when the switch is pressed and turned OFF when the switch is released

| Board      | Switch Name | LED Name Name
| ----------------- | ---------- | ---------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | SW0 | LED1 |
| [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)      |SW0  | LED0 |
