---
parent: Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family
title: SPI ping pong with DMA 
has_children: false
has_toc: false
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# SPI ping pong with DMA

This example shows how to continuously transmit and receive data over a SPI interface using ping pong buffers with the DMA.

## Description

DMA peripheral provides support to implement ping-pong buffering. DMA transmits and receives data from one pair of buffers, when CPU works on the second pair.

DMA uses linked list to continuously transfer data from ping and pong buffers. Linked list descriptor 0 is configured to transfer data from ping buffer. The linked list descriptor 0 points to descriptor 1 which is configured to transfer data from the pong buffer. The descriptor 1 is configured to point back to descriptor 0.

DMA generates an interrupt when it completes transferring data from ping buffer and starts transferring data from the pong buffer. While DMA is transferring data from pong buffer, CPU works on the ping buffer. When DMA completes transferring data from pong buffer it generates an interrupt and starts transferring data from the ping buffer. While DMA is transferring data from ping buffer, CPU works on the pong buffer.

Data is transmitted and received in loopback mode by shorting the MISO and MOSI lines. At the end of transfer, received data is compared with transmitted data to confirm data match.

## Downloading and building the application

To clone or download this application from Github, go to the [main page of this repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) and then click **Clone** button to clone this repository or download as zip file.
This content can also be downloaded using content manager by following these [instructions](https://github.com/Microchip-MPLAB-Harmony/contentmanager/wiki).

Path of the application within the repository is **apps/spi/spi_ping_pong_with_dma/firmware** .

To build the application, refer to the following table and open the project using its IDE.

| Project Name      | Description                                    |
| ----------------- | ---------------------------------------------- |
| sam_e70_xult.X or sam_e70_xult_iar.IAR/spi_ping_pong_with_dma_sam_e70_xult.eww | MPLABX or IAR Project for [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
| sam_v71_xult.X    | MPLABX Project for  [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|
|||

## Setting up the hardware

The following table shows the target hardware for the application projects.

| Project Name| Board|
|:---------|:---------:|
|sam_e70_xult.X or sam_e70_xult_iar.IAR/spi_ping_pong_with_dma_sam_e70_xult.eww | [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)|
|sam_v71_xult.X | [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)|
|||

### Setting up [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)

- Use a jumper wire to short pin 16 and pin 17 of the EXT1 header
- Connect the Debug USB port on the board to the computer using a micro USB cable

### Setting up [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)

- Use a jumper wire to short pin 16 and pin 17 of the EXT1 header
- Connect the Debug USB port on the board to the computer using a micro USB cable

## Running the Application

1. Build and Program the application project using its IDE
2. LED indicates the success or failure:
    - The LED is turned ON when the value received matched with the transmitted value
    - The LED is turned OFF when the value received did not match with the transmitted value

The following table provides the LED name.

| Board      | LED |
| ----------------- | ---------------------------------------------- |
| [SAM E70 Xplained Ultra board](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320113)    | LED1 |
| [SAM V71 Xplained Ultra board](https://www.microchip.com/developmenttools/ProductDetails/atsamv71-xult)      | LED0 |
|||
