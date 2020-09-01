---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title: MCAN FD interrupt 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# MCAN FD interrupt

This example shows how to use the MCAN module to transmit and receive CAN FD messages in interrupt mode.

## Description

This application transmits and receives CAN FD messages on the CAN bus. To run this application, two evaluation boards of same type are required. These boards acts as different nodes on the CAN bus. Same application is programmed onto both the boards. Boards are connected to PC via UART. While running the application, user can send and receive CAN FD messages between the boards using UART console applications running on the PC.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/mcan/mcan_fd_operation_interrupt_timestamp/firmware** .

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

- Connect SAM E70 Xplained Ultra board to another SAM E70 Xplained Ultra board as per the pin connections shown below

| SAME70 XULT - 1   | SAME70 XULT - 2    |
| ----------------- | ------------------ |
| CAN_H             | CAN_H              |
| CAN_L             | CAN_L              |
| GND               | GND                |
|||

- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Connect SAM V71 Xplained Ultra board to another SAM V71 Xplained Ultra board as per the pin connections shown below

| SAMV71 XULT - 1   | SAMV71 XULT - 2    |
| ----------------- | ------------------ |
| CAN_H             | CAN_H              |
| CAN_L             | CAN_L              |
| GND, EXT1 PIN19   | GND, EXT1 PIN19    |
|||

- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Open the Terminal application (Ex.:Tera term) on the computer
2. Connect to the EDBG Virtual COM port of both the boards and configure the serial settings as follows:
    - Baud : 115200
    - Data : 8 Bits
    - Parity : None
    - Stop : 1 Bit
    - Flow Control : None
3. Build and Program the application on both the boards using their respective IDEs
4. In the console associated with board 2, press "0" to receive a CAN FD message
5. In the console associated with board 1, press "2" to transmit a CAN FD message
6. Transmitted message description and status will be displayed in the console window of board
7. Received message will be displayed in the console window of board 2
8. Follow the sequence below to send and receive different messages:
    - Press '1' on board 2 and Press '3' on board 1
    - Press '4' on board 2 and Press '6' on board 1
    - Press '5' on board 2 and Press '7' on board 1
    - Press '1' on board 2 and Press 'a' on board 1
9. If the steps are executed in this sequence, the final output in the consoles will be as below (console on the left is the transmitter (board 1) and the one on the right is receiver (board 2)):
![output](images/output_mcan_fd_operation_interrupt.png)
