/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi0_slave.c

  Summary:
    SPI0 Slave Source File

  Description:
    This file has implementation of all the interfaces provided for particular
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

#include "plib_spi0_slave.h"
#include "peripheral/pio/plib_pio.h"
#include <string.h>

#define SPI0_READ_BUFFER_SIZE            256
#define SPI0_WRITE_BUFFER_SIZE           256

static uint8_t SPI0_ReadBuffer[SPI0_READ_BUFFER_SIZE];
static uint8_t SPI0_WriteBuffer[SPI0_WRITE_BUFFER_SIZE];


// *****************************************************************************
// *****************************************************************************
// Section: SPI0 Implementation
// *****************************************************************************
// *****************************************************************************

/* Global object to save SPI Exchange related data */
static SPI_SLAVE_OBJECT spi0Obj;

void SPI0_Initialize( void )
{
    /* Disable and Reset the SPI*/
    SPI0_REGS->SPI_CR = SPI_CR_SPIDIS_Msk | SPI_CR_SWRST_Msk;


    /* SPI is by default in Slave Mode, disable mode fault detection */
    SPI0_REGS->SPI_MR = SPI_MR_MODFDIS_Msk;

    /* Set up clock Polarity, data phase, Communication Width */
    SPI0_REGS->SPI_CSR[0] = SPI_CSR_CPOL_IDLE_LOW | SPI_CSR_NCPHA_VALID_LEADING_EDGE | SPI_CSR_BITS_8_BIT;

    spi0Obj.rdInIndex = 0;
    spi0Obj.wrOutIndex = 0;
    spi0Obj.nWrBytes = 0;
    spi0Obj.errorStatus = SPI_SLAVE_ERROR_NONE;
    spi0Obj.callback = NULL ;
    spi0Obj.transferIsBusy = false ;

    /* Set the Busy Pin to ready state */
    PIO_PinWrite((PIO_PIN)PIO_PIN_PD28, 0);

    /* Enable Receive full and chip deselect interrupt */
    SPI0_REGS->SPI_IER = (SPI_IER_RDRF_Msk | SPI_IER_NSSR_Msk);

    /* Enable SPI0 */
    SPI0_REGS->SPI_CR = SPI_CR_SPIEN_Msk;
}

/* For 9-bit mode, the "size" must be specified in terms of 16-bit words */
size_t SPI0_Read(void* pRdBuffer, size_t size)
{
    size_t rdSize = size;
    size_t rdInIndex = spi0Obj.rdInIndex;

    if (rdSize > rdInIndex)
    {
        rdSize = rdInIndex;
    }
    memcpy(pRdBuffer, SPI0_ReadBuffer, rdSize);

    return rdSize;
}

/* For 9-bit mode, the "size" must be specified in terms of 16-bit words */
size_t SPI0_Write(void* pWrBuffer, size_t size )
{
    uint32_t intState = SPI0_REGS->SPI_IMR;
    size_t wrSize = size;

    SPI0_REGS->SPI_IDR = intState;

    if (wrSize > SPI0_WRITE_BUFFER_SIZE)
    {
        wrSize = SPI0_WRITE_BUFFER_SIZE;
    }

    memcpy(SPI0_WriteBuffer, pWrBuffer, wrSize);

    spi0Obj.nWrBytes = wrSize;
    spi0Obj.wrOutIndex = 0;

    while ((SPI0_REGS->SPI_SR & SPI_SR_TDRE_Msk) && (spi0Obj.wrOutIndex < spi0Obj.nWrBytes))
    {
        *((uint8_t*)&SPI0_REGS->SPI_TDR) = SPI0_WriteBuffer[spi0Obj.wrOutIndex++];
    }

    /* Restore interrupt enable state and also enable TDRE interrupt */
    SPI0_REGS->SPI_IER = (intState | SPI_IER_TDRE_Msk);

    return wrSize;
}

/* For 9-bit mode, the return value is in terms of 16-bit words */
size_t SPI0_ReadCountGet(void)
{
    return spi0Obj.rdInIndex;
}

/* For 9-bit mode, the return value is in terms of 16-bit words */
size_t SPI0_ReadBufferSizeGet(void)
{
    return SPI0_READ_BUFFER_SIZE;
}

/* For 9-bit mode, the return value is in terms of 16-bit words */
size_t SPI0_WriteBufferSizeGet(void)
{
    return SPI0_WRITE_BUFFER_SIZE;
}

void SPI0_CallbackRegister(SPI_SLAVE_CALLBACK callBack, uintptr_t context )
{
    spi0Obj.callback = callBack;

    spi0Obj.context = context;
}

/* The status is returned busy during the period the chip-select remains asserted */
bool SPI0_IsBusy(void)
{
    return spi0Obj.transferIsBusy;
}

/* Drive the GPIO pin to indicate to SPI Master that the slave is ready now */
void SPI0_Ready(void)
{
    PIO_PinWrite((PIO_PIN)PIO_PIN_PD28, 0);
}

SPI_SLAVE_ERROR SPI0_ErrorGet(void)
{
    SPI_SLAVE_ERROR errorStatus = spi0Obj.errorStatus;

    spi0Obj.errorStatus = SPI_SLAVE_ERROR_NONE;

    return errorStatus;
}

void SPI0_InterruptHandler(void)
{
    uint8_t txRxData = 0;

    uint32_t statusFlags = SPI0_REGS->SPI_SR;

    if (statusFlags & SPI_SR_OVRES_Msk)
    {
        /*OVRES flag is cleared on reading SPI SR*/

        /* Save the error to report it to application later */
        spi0Obj.errorStatus = SPI_SR_OVRES_Msk;
    }

    if(statusFlags & SPI_SR_RDRF_Msk)
    {
        if (spi0Obj.transferIsBusy == false)
        {
            spi0Obj.transferIsBusy = true;

            PIO_PinWrite((PIO_PIN)PIO_PIN_PD28, 1);
        }

        /* Note: statusFlags must be updated every time SPI_SR is read. This is because the NSSR flag
         * is cleared on SPI_SR read. If statusFlags is not updated, there is a possibility of missing
         * NSSR event flag.
         */
        while ((statusFlags |= SPI0_REGS->SPI_SR) & SPI_SR_RDRF_Msk)
        {
            /* Reading DATA register will also clear the RDRF flag */
            txRxData = *((uint8_t*)&SPI0_REGS->SPI_RDR);

            if (spi0Obj.rdInIndex < SPI0_READ_BUFFER_SIZE)
            {
                SPI0_ReadBuffer[spi0Obj.rdInIndex++] = txRxData;
            }

            /* Only clear RDRF flag so as not to clear NSSR flag which may have been set */
            statusFlags &= ~SPI_SR_RDRF_Msk;
        }
    }

    if(statusFlags & SPI_SR_TDRE_Msk)
    {
        while (((statusFlags |= SPI0_REGS->SPI_SR) & SPI_SR_TDRE_Msk) && (spi0Obj.wrOutIndex < spi0Obj.nWrBytes))
        {
            *((uint8_t*)&SPI0_REGS->SPI_TDR) = SPI0_WriteBuffer[spi0Obj.wrOutIndex++];
            /* Only clear TDRE flag so as not to clear NSSR flag which may have been set */
            statusFlags &= ~SPI_SR_TDRE_Msk;
        }

        if (spi0Obj.wrOutIndex >= spi0Obj.nWrBytes)
        {
            /* Disable TDRE interrupt. The last byte sent by the master will be shifted out automatically */
            SPI0_REGS->SPI_IDR = SPI_IDR_TDRE_Msk;
        }
    }

    if(statusFlags & SPI_SR_NSSR_Msk)
    {
        /* NSSR flag is cleared on reading SPI SR */

        spi0Obj.transferIsBusy = false;

        spi0Obj.wrOutIndex = 0;
        spi0Obj.nWrBytes = 0;

        if(spi0Obj.callback != NULL)
        {
            spi0Obj.callback(spi0Obj.context);
        }

        /* Clear the rdInIndex. Application must read the received data in the callback. */
        spi0Obj.rdInIndex = 0;
    }
}