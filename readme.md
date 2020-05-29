[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 peripheral library application examples for SAM E70/S70/V70/V71 family

- [Release Notes](release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)

To download or clone these application from Github, go to the [top level of the repository](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71) page and click:

![clone](./docs/images/clone.png)

## Contents Summary

| Folder     | Description                             |
| ---        | ---                                     |
| apps       | Peripheral library example applications |
| docs       | Applications help documentation         |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries.

| App Name | Description|
|:---------|:-----------|
|[Analog Comparator Controller PWM example (acc_wave_gen)](apps/acc/acc_wav_gen/readme.md) | This application example uses ACC peripheral Library to generate PWM waveform with variable duty cycle|
|[Analog Front-End Controller DMA striding example (afec_dma_striding)](apps/afec/afec_dma_striding/readme.md) | This application example uses AFEC peripheral Library to sample multiple ADC channels using DMA striding|
|[Analog Front-End Controller polling mode example (afec_polled_mode)](apps/afec/afec_polled_mode/readme.md) | This example uses AFEC peripheral library to demonstrate how to sample an analog input in polled mode and send the converted data to console|
|[Analog Front-End Controller user sequencer Example (afec_user_sequence)](apps/afec/afec_user_sequence/readme.md) | This example uses AFEC peripheral library to demonstrate how to sample three analog inputs using the user sequencer in triggered conversion mode and send the converted data to the console|
|[Cache maintenance operations example (cache_maintainenace)](apps/cache/cache_maintenance/readme.md) | This example demonstrates the cache maintenance operation by cleaning and invalidating the cache for the DMA buffers located in the cacheable SRAM region|
|[Clock configurations example (clock_config)](apps/clock/clock_config/readme.md) | This example application demonstrates how to configure the clock system to run the device at maximum frequency|
|[Digital to Analog Converter Controller waveform generation example (dacc_wav_gen)](apps/dacc/dacc_wav_gen/readme.md) | This example application demonstrates how to configure the clock system to run the device at maximum frequency|
|[Digital to Analog Converter Controller waveform generation using dma example (dacc_wav_gen_dma)](apps/dacc/dacc_wav_gen_dma/readme.md) | This example shows how to use the DAC with the DMA to generate sinusoidal waveform without CPU intervention|
|[Embedded Flash Controller flash read write example (flash_read_write)](apps/efc/flash_read_write/readme.md) | This example application demonstrates how to use the EFC to erase and program the internal Flash memory|
|[Controller Area Network fd blocking operation example (mcan_fd_operation_blocking)](apps/mcan/mcan_fd_operation_blocking/readme.md) | This example application shows how to use the MCAN module to transmit and receive CAN FD messages in polling mode|
|[Controller Area Network fd interrupt operation timestamp example (mcan_fd_operation_interrupt_timestamp)](apps/mcan/mcan_fd_operation_interrupt_timestamp/readme.md) | This example application shows how to use the MCAN module to transmit and receive CAN FD messages in interrupt mode|
|[Controller Area Network normal blocking operation example (mcan_normal_operation_blocking)](apps/mcan/mcan_normal_operation_blocking/readme.md) | This example application shows how to use the MCAN module to transmit and receive normal CAN messages in polling mode|
|[Memory Protection Unit (MPU) cache coherent memory example(mpu_coherent_regions)](apps/mpu/mpu_coherent_region/readme.md) | This example demonstrates maintaining cache coherency by allocating DMA buffers in non-cacheable SRAM region defined by MPU|
|[Parallel Input/Output Controller LED on/off using interrupt example (pio_led_on_off_interrupt)](apps/pio/pio_led_on_off_interrupt/readme.md) | This example application demonstrate how to generate GPIO interrupt on switch press and release, and indicate the switch status using the LED|
|[Parallel Input/Output Controller LED on/off using polling example (pio_led_on_off_polling)](apps/pio/pio_led_on_off_polling/readme.md) | This example application demonstrate how to poll the switch input, and indicate the switch status using the LED|
|[Pulse width modulation 3 phase waveform generation using synchronous channels (pwm_synchronous_channels)](apps/pwm/pwm_synchronous_channels/readme.md) | This example demonstrates how to use the PWM peripheral to generate 3-phase PWM signals with dead time |
|[Quad Serial Peripheral Interface (QSPI) flash read write example (qspi_read_write)](apps/qspi/qspi_read_write/readme.md) | This example application demonstrates how to use the QSPI Peripheral library to perform erase, write and read operation with the QSPI Serial Flash memory in Quad IO mode |
|[Quad Serial Peripheral Interface (QSPI) flash XIP example (qspi_xip)](apps/qspi/qspi_xip/readme.md) | This example application demonstrates how to setup the QSPI Flash in the XIP mode to execute code from the serial Flash memory |
|[Quad Serial Peripheral Interface (QSPI) flash read write in SPI mode example (spi_read_write)](apps/qspi/spi_read_write/readme.md) | This example application demonstrates how to use the QSPI Peripheral library to perform erase, write and read operation with the QSPI Serial Flash memory in SPI mode |
|[Reset Controller interrupt generation example (led_switcher)](apps/rstc/led_switcher/readme.md) | This example application demonstrates how to use the RSTC peripheral to generate an interrupt instead of generating a reset, when the Reset switch is pressed on board |
|[Reinforced Safety Watchdog Timer (RSWDT) timeout application example (rswdt_timeout)](apps/rswdt/rswdt_timeout/readme.md) | This example demonstrates Reinforced Safety Watchdog Timer reset by emulating a deadlock |
|[Real Time Clock (RTC) Alarm generation application example (rtc_alarm)](apps/rtc/rtc_alarm/readme.md) | This example application shows how to use the RTC to configure the time and generate the alarm |
|[Real Time Timer (RTT) periodic timer example (rtt_periodic_timeout)](apps/rtt/rtt_periodic_timeout/readme.md) | This example application shows how to use the RTT to generate periodic interrupts |
|[SDRAM read write application example (sdramc_read_write)](apps/sdramc/sdramc_read_write/readme.md) | This example application demonstrates how to configure the external SDRAM using SDRAM controller to write and read data |
|[SDRAM read write performance application example (sdramc_read_write_perf)](apps/sdramc/sdramc_read_write_perf/readme.md) | This example application demonstrates how to configure the SDRAM interface and measures the read/write throughput with DMA |
|[SPI EEPROM read write example (spi_eeprom_write_read)](apps/spi/spi_eeprom_write_read/readme.md) | This example application demonstrates how to use the SPI peripheral to write and read from the SPI serial EEPROM memory |
|[SPI ping pong with dma example (spi_ping_pong_with_dma)](apps/spi/spi_ping_pong_with_dma/readme.md) | This example demonstrates how to continuously transmit and receive data over a SPI interface using ping pong buffers with the DMA |
|[SPI self loopback data transfer in blocking mode example (spi_self_loopback_blocking)](apps/spi/spi_self_loopback_blocking/readme.md) | This example application demonstrates how to use the SPI peripheral to transmit and receive a block of data in a blocking manner |
|||
____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71/releases/latest)
[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg)](https://github.com/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/csp_apps_sam_e70_s70_v70_v71.svg?style=social)]()
