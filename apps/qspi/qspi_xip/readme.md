[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# QSPI flash XIP

This example shows how to setup the QSPI Flash in the XIP mode to execute code from the serial Flash memory.

## Description

The QSPI execute in place (XIP) feature allows the system to execute code directly from a serial Flash memory without code shadowing to RAM. The serial Flash memory appears as memory-mapped device in QSPI region. As Quad SPI mode uses four lines for data transfer, it allows the system to use high-performance serial Flash memories which are small and inexpensive, in place of larger and more expensive parallel Flash memories.

### QSPI XIP Image Application

To be able to execute from QSPI memory space, the binary file of the application needs to be linked to the QSPI address space. A sample QSPI XIP image application is provided using the custom linker script to blink the LED continuously. After generating the binary file using the XIP image application, a custom python script is used to extract the hex code from the binary and store it in the header file as an array of hex values.

### QSPI Main Application

The main application includes the header file containing the firmware image. It programs and execute code from serial Flash memory. The main application execution sequence is given below:

- Program the QSPI XIP image in serial Flash memory
- Enable 'Continuous Read Mode' to enter into XIP mode
- Verify the image by reading the values back and comparing it to the value written
- If verification passes, configure the Main Stack Pointer and jump to the application in the QSPI based on the value read from the QSPI region.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and click

![clone](../../../docs/images/clone.png)

Path of the application within the repository is **apps/qspi/qspi_xip/** .

To build the application, refer to the following table and open the project using its IDE.

### Main Application

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| xip_main/firmware/sam_e70_xult.X    | MPLABX Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|||

### XIP Application

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| xip_image/firmware/sam_e70_xult.X    | MPLABX Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
|sam_e70_xult.X | [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|||

### Setting up [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Open the QSPI XIP image project *xip_image/firmware/sam_e70_xult.X* in the IDE
2. Build the project to generate the binary (Do not program the binary)
3. Run the python script  *xip_image_pattern_gen.py* from command prompt (script uses relative paths, so run the script from application path i.e. apps/qspi/qspi_xip/) which does the following:
    - Extracts the executable code from output of step 2
    - Convert the executable code into a array of hex values
    - Store the array in  the header file *xip_image_pattern_hex.h* (in xip_main/firmware/src/config/sam_e70_xult folder)
4. Open the QSPI main project *xip_main/firmware/ sam_e70_xult.X* in the IDE
5. Build and program the application using the IDE
6. The main application programs the QSPI XIP image and executes the code from serial Flash memory
7. The QSPI XIP image application blinks the LED continuously.

The following table provides the LED name

| Board      | LED name |
| ----------------- | ---------------------------------------------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | LED1 |
|||
