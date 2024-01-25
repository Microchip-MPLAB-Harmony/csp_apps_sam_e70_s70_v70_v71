---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title:  USART LIN Slave 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# USART LIN Slave

This example application demonstrates how to use the USART peripheral in LIN Slave mode

## Description

In this example, the LIN Master reads LED and Switch status of the LIN slave every 1000 milliseconds by sending a LIN receive command. The LIN slave responds with 1 byte data containing the Switch status (Bit[0]) and LED status (Bit[1]). 

Upon receiving the response, LIN Master checks if the Switch status is in pressed or released state. If the switch is pressed, the LIN Master toggles the state of receieved LED status and asks the LIN slave to set the LED to the new state (set LED to OFF state if LED was previously ON and vice-versa) by sending a LIN transmit command.

PID for reading LED and Switch status - 0x80 (Frame ID - 0x00). 

PID for writing LED status - 0xC1 (Frame ID - 0x01). 

The example uses enhanced checksum.

## Downloading and building the application

To download or clone this application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.


This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/usart/usart_lin/slave/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X    | MPLABX Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
|sam_e70_xult.X | [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|||

Hardware setup requires two boards, one acting as the LIN Master and the other as the LIN slave.

### Setting up [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Connect a jumper between pin1 (LED2) and pin2 (PB08) on J203 connector on the slave board.  
- Plug an [LIN Click board](https://www.mikroe.com/lin-click) into the MikroBus socket on the board. 
- Slave node should be enabled by proper configuration of MODE resistor on the click board.
- Click board should be enabled for 5V mode , by proper configuraion of VCC SEL resistor on the click board  
- Connect a wire from "BUS" on LIN Master click board to "BUS" on LIN slave click board
- Connect a wire from "GND" on LIN Master click board to "GND" on LIN slave click board
- Connect a wire from "VS" on LIN Master click board to "VS" on LIN slave click board
- Connect a wire from "VS" on LIN Master click board to 5V.
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program LIN master and LIN slave applications using its IDE. Path of the LIN Slave application within the repository is **apps/usart/usart_lin/master/firmware**.
2. Press switch on LIN Master board to start the communication. LIN Master will start reading LED and Switch status from the LIN slave every 1000 milliseconds.
3. Press switch on the LIN Slave board. The LED on the LIN Slave board will toggle if the switch is pressed.
4. The following table provides the LED and Switch names

| Board      | LED Name  | Switch Name |
| ----------------- | ----------- | ----------- |
| [SAME70 Xplained Ultra Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM320113) |LED2 | SW400 |
|||
