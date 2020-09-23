/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi0_slave.h

  Summary:
    SPI0 SLAVE PLIB Header File

  Description:
    This file has prototype of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_SPI0_SLAVE_H
#define PLIB_SPI0_SLAVE_H

#include "device.h"
#include "plib_spi_slave_common.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus

    extern "C" {

#endif

/****************************** SPI0 Interface *********************************/

void SPI0_Initialize (void);
size_t SPI0_Read(void* pRdBuffer, size_t size);
size_t SPI0_Write(void* pWrBuffer, size_t size );
size_t SPI0_ReadCountGet(void);
size_t SPI0_ReadBufferSizeGet(void);
size_t SPI0_WriteBufferSizeGet(void);
void SPI0_CallbackRegister(SPI_SLAVE_CALLBACK callBack, uintptr_t context );
SPI_SLAVE_ERROR SPI0_ErrorGet(void);
bool SPI0_IsBusy(void);
void SPI0_Ready(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus

    }

#endif

#endif // PLIB_SPI0_SLAVE_H

/*******************************************************************************
 End of File
*/
