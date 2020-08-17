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

| Name | Description|
|:---------|:-----------|
|[ACC PWM generation](apps/acc/acc_wav_gen/readme.md) | This example uses ACC peripheral Library to generate a PWM waveform with variable duty cycle|
|[AFEC with DMA](apps/afec/afec_dma_striding/readme.md) | This example uses AFEC peripheral Library to sample multiple ADC channels using DMA striding|
|[AFEC Polling](apps/afec/afec_polled_mode/readme.md) | This example uses AFEC peripheral library to sample an analog input in polled mode and send the converted data to console|
|[AFEC user sequence](apps/afec/afec_user_sequence/readme.md) | This example uses AFEC peripheral library to sample three analog inputs using the user sequencer in triggered conversion mode and send the converted data to the console|
|[Cache maintenance](apps/cache/cache_maintenance/readme.md) | This example shows cache maintenance operation by cleaning and invalidating the cache for the DMA buffers located in the cacheable SRAM region|
|[Clock configuration](apps/clock/clock_config/readme.md) | This example shows how to configure the clock system to run the device at maximum frequency|
|[DACC waveform generation](apps/dacc/dacc_wav_gen/readme.md) | This example shows how to use the DACC peripheral to generate a 5 KHz sine wave or triangle wave using a lookup table|
|[DACC waveform generation with DMA](apps/dacc/dacc_wav_gen_dma/readme.md) | This example shows how to use the DAC with the DMA to generate sinusoidal waveform without CPU intervention|
|[EFC read write](apps/efc/flash_read_write/readme.md) | This example shows how to use EFC to erase and program the internal Flash memory|
|[MCAN FD blocking](apps/mcan/mcan_fd_operation_blocking/readme.md) | This example shows how to use the MCAN module to transmit and receive CAN FD messages in polling mode|
|[MCAN FD interrupt](apps/mcan/mcan_fd_operation_interrupt_timestamp/readme.md) | This example shows how to use the MCAN module to transmit and receive CAN FD messages in interrupt mode|
|[MCAN blocking](apps/mcan/mcan_normal_operation_blocking/readme.md) | This example shows how to use the MCAN module to transmit and receive normal CAN messages in polling mode|
|[MPU cache coherency](apps/mpu/mpu_coherent_region/readme.md) | This example shows how to maintain cache coherency by allocating DMA buffers in non-cacheable SRAM region defined by MPU|
|[PIO interrupt](apps/pio/pio_led_on_off_interrupt/readme.md) | This example shows how to generate GPIO interrupt on switch press and release, and indicate the switch status using the LED|
|[PIO polling](apps/pio/pio_led_on_off_polling/readme.md) | This example shows how to poll the switch input, and indicate the switch status using the LED|
|[PWM Generation](apps/pwm/pwm_synchronous_channels/readme.md) | This example shows how to use the PWM peripheral to generate 3-phase PWM signals with dead time |
|[QSPI flash read write](apps/qspi/qspi_read_write/readme.md) | This example shows how to use the QSPI Peripheral library to perform erase, write and read operation with the QSPI Serial Flash memory in Quad IO mode |
|[QSPI flash XIP](apps/qspi/qspi_xip/readme.md) | This example shows how to setup the QSPI Flash in the XIP mode to execute code from the serial Flash memory |
|[QSPI flash XIP Function](apps/qspi/qspi_xip_function/readme.md) | This example shows how to setup the QSPI Flash in the XIP mode to call a function placed in QSPI Flash memory |
|[QSPI flash read write in SPI mode](apps/qspi/spi_read_write/readme.md) | This example application shows how to use the QSPI Peripheral library to perform erase, write and read operation with the QSPI Serial Flash memory in SPI mode |
|[RSTC interrupt](apps/rstc/led_switcher/readme.md) | This example shows how to use the RSTC peripheral to generate an interrupt instead of generating a reset, when the Reset switch is pressed on board |
|[RSWDT timeout](apps/rswdt/rswdt_timeout/readme.md) | This example shows how to generate a RSWDT reset by emulating a deadlock |
|[RTC alarm interrupt](apps/rtc/rtc_alarm/readme.md) | This example shows how to use the RTC to configure the time and generate the alarm |
|[RTT periodic interrupt](apps/rtt/rtt_periodic_timeout/readme.md) | This example shows how to use the RTT to generate periodic interrupts |
|[SDRAM read write](apps/sdramc/sdramc_read_write/readme.md) | This example shows how to configure the external SDRAM using SDRAM controller to write and read data |
|[SDRAM read write performance](apps/sdramc/sdramc_read_write_perf/readme.md) | This example shows how to configure the SDRAM interface and measures the read/write throughput with DMA |
|[SPI EEPROM read write](apps/spi/spi_eeprom_write_read/readme.md) | This example shows how to use the SPI peripheral to write and read from the SPI serial EEPROM memory |
|[SPI ping pong with DMA](apps/spi/spi_ping_pong_with_dma/readme.md) | This example shows how to continuously transmit and receive data over a SPI interface using ping pong buffers with the DMA |
|[SPI blocking](apps/spi/spi_self_loopback_blocking/readme.md) | This example shows how to use the SPI peripheral to transmit and receive a block of data in a blocking manner |
|[SPI interrupt](apps/spi/spi_self_loopback_interrupt/readme.md) | This example application demonstrates how to use the SPI peripheral to transmit and receive a block of data using interrupts |
|[SUPC wakeup using RTC](apps/supc/supc_wakeup_rtc/readme.md) | This example shows how to enter different low power modes and exit it using the RTC alarm |
|[SUPC wakeup using RTT](apps/supc/supc_wakeup_rtt/readme.md) | This example shows how to enter different low power modes and exit it using the RTT alarm |
|[SUPC wakeup using WKUP pin](apps/supc/supc_wakeup_wkup/readme.md) | This example shows how to enter different low power modes and exit it using WKUP pin |
|[SysTick periodic interrupt](apps/systick/systick_periodic_timeout/readme.md) | This example shows how to use the SysTick to generate periodic interrupts |
|[TC capture mode](apps/tc/tc_capture_mode/readme.md) | This example shows how to use the TC module in capture mode to measure duty cycle and frequency of an external input |
|[TC compare mode](apps/tc/tc_compare_mode/readme.md) | This example shows how to use the TC module in compare mode to generate an active low, active high, and toggle output on compare match |
|[TC timer mode](apps/tc/tc_timer_mode/readme.md) | This example demonstrates how to use the TC module in timer mode to generate periodic interrupt |
|[TCM cache coherency](apps/tcm/data_tcm/readme.md) | This example shows how to  maintain cache coherency by allocating the DMA buffers in the Tightly Coupled Memory (TCM), which are not cacheable |
|[TRNG random number](apps/trng/random_number/readme.md) | This example shows how to use the TRNG Peripheral library to generate and read a random number |
|[TWIHS EEPROM read write](apps/twihs/master/twihs_eeprom/readme.md) | This example shows how to use the TWIHS peripheral to write and read from the I2C serial EEPROM memory |
|[USART blocking](apps/usart/usart_echo_blocking/readme.md) | This example shows how to use the USART peripheral to transfer a block of data in a blocking manner |
|[USART interrupt](apps/usart/usart_echo_interrupt/readme.md) | This example shows how to use the USART peripheral to transfer a block of data in a non-blocking manner |
|[USART ring buffer](apps/usart/usart_ring_buffer_interrupt/readme.md) | This example shows how to use the USART peripheral in ring buffer mode |
|[WDT timeout](apps/wdt/wdt_timeout/readme.md) | TThis example shows how to generate a Watchdog timer reset by emulating a deadlock |
|[XDMAC memory transfer](apps/xdmac/xdmac_memory_transfer/readme.md) | This example shows how to use the XDMAC peripheral to do a memory to memory transfer |
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
