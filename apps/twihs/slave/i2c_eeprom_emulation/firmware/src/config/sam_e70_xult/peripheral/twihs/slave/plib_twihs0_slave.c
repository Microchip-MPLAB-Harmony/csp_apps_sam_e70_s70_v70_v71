/*******************************************************************************
  TWIHS Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_twihs0_slave.c

  Summary
    TWIHS Slave peripheral library interface.

  Description

  Remarks:

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_twihs0_slave.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

static TWIHS_SLAVE_OBJ twihs0Obj;

// *****************************************************************************
// *****************************************************************************
// TWIHS0 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

void TWIHS0_Initialize( void )
{
    /* Reset the TWIHS Module */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    /* Disable the TWIHS Master/Slave Mode */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    /* Configure slave address */
    TWIHS0_REGS->TWIHS_SMR = TWIHS_SMR_SADR(0x54);

    /* Clear the Transmit Holding Register and set TXRDY, TXCOMP flags */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_THRCLR_Msk;

    /* Initialize the TWIHS PLIB Object */
    /* Enable interrupt for Slave Access and End of Slave Access */
    TWIHS0_REGS->TWIHS_IER = TWIHS_IER_SVACC_Msk | TWIHS_IER_EOSACC_Msk;

    twihs0Obj.callback = NULL;
    twihs0Obj.isBusy = false;
    twihs0Obj.isFirstTxPending = false;
    twihs0Obj.isAddrMatchEventNotified = false;

    /* Enable Slave Mode */
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_SVEN_Msk;
}

uint8_t TWIHS0_ReadByte(void)
{
    return (uint8_t)(TWIHS0_REGS->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);
}

void TWIHS0_WriteByte(uint8_t wrByte)
{
    TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(wrByte);
}

TWIHS_SLAVE_TRANSFER_DIR TWIHS0_TransferDirGet(void)
{
    return (TWIHS0_REGS->TWIHS_SR & TWIHS_SR_SVREAD_Msk)? TWIHS_SLAVE_TRANSFER_DIR_READ: TWIHS_SLAVE_TRANSFER_DIR_WRITE;
}

TWIHS_SLAVE_ACK_STATUS TWIHS0_LastByteAckStatusGet(void)
{
    return (TWIHS0_REGS->TWIHS_SR & TWIHS_SR_NACK_Msk)? TWIHS_SLAVE_ACK_STATUS_RECEIVED_NAK : TWIHS_SLAVE_ACK_STATUS_RECEIVED_ACK;
}

void TWIHS0_NACKDataPhase(bool isNACKEnable)
{
    if (isNACKEnable == true)
    {
        TWIHS0_REGS->TWIHS_SMR = (TWIHS0_REGS->TWIHS_SMR & ~TWIHS_SMR_NACKEN_Msk) | TWIHS_SMR_NACKEN_Msk;
    }
    else
    {
        TWIHS0_REGS->TWIHS_SMR = (TWIHS0_REGS->TWIHS_SMR & ~TWIHS_SMR_NACKEN_Msk);
    }
}


void TWIHS0_CallbackRegister(TWIHS_SLAVE_CALLBACK callback, uintptr_t contextHandle)
{
    twihs0Obj.callback = callback;

    twihs0Obj.context  = contextHandle;
}

bool TWIHS0_IsBusy(void)
{
    return twihs0Obj.isBusy;
}

void TWIHS0_InterruptHandler( void )
{
    uint32_t status = TWIHS0_REGS->TWIHS_SR;

    if (status & TWIHS0_REGS->TWIHS_IMR)
    {
        if(status & TWIHS_SR_SVACC_Msk)
        {
            if (twihs0Obj.isAddrMatchEventNotified == false)
            {
                TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_SVACC_Msk;

                if (TWIHS0_TransferDirGet() == TWIHS_SLAVE_TRANSFER_DIR_READ)
                {
                    TWIHS0_REGS->TWIHS_IER = TWIHS_IER_TXRDY_Msk | TWIHS_IER_TXCOMP_Msk;

                    /* Set flag so that we do not check for NAK from master before transmitting the first byte */
                    twihs0Obj.isFirstTxPending = true;
                }
                else
                {
                    TWIHS0_REGS->TWIHS_IER = TWIHS_IER_RXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
                }

                twihs0Obj.isBusy = true;

                if (twihs0Obj.callback != NULL)
                {
                    twihs0Obj.callback(TWIHS_SLAVE_TRANSFER_EVENT_ADDR_MATCH, twihs0Obj.context);
                }

                twihs0Obj.isAddrMatchEventNotified = true;
            }

            /* I2C Master reads from slave */
            if (TWIHS0_TransferDirGet() == TWIHS_SLAVE_TRANSFER_DIR_READ)
            {
                if (status & TWIHS_SR_TXRDY_Msk)
                {
                    if ((twihs0Obj.isFirstTxPending == true) || (!(status & TWIHS_SR_NACK_Msk)))
                    {
                        if (twihs0Obj.callback != NULL)
                        {
                            twihs0Obj.callback(TWIHS_SLAVE_TRANSFER_EVENT_TX_READY, twihs0Obj.context);
                        }
                        twihs0Obj.isFirstTxPending = false;
                    }
                    else
                    {
                        /* NAK received. Turn off TXRDY interrupt. */
                        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
                    }
                }
            }
            else
            {
                /* I2C Master writes to slave */
                if (status & TWIHS_SR_RXRDY_Msk)
                {
                    if (twihs0Obj.callback != NULL)
                    {
                        twihs0Obj.callback(TWIHS_SLAVE_TRANSFER_EVENT_RX_READY, twihs0Obj.context);
                    }
                }
            }
        }
        else if (status & TWIHS_SR_EOSACC_Msk)
        {
            /* Either Repeated Start or Stop condition received */

            twihs0Obj.isAddrMatchEventNotified = false;

            TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;
            TWIHS0_REGS->TWIHS_IER = TWIHS_IER_SVACC_Msk;

            if (status & TWIHS_SR_TXCOMP_Msk)
            {
                /* Stop condition received OR start condition with other slave address detected */

                twihs0Obj.isBusy = true;

                if (twihs0Obj.callback != NULL)
                {
                    twihs0Obj.callback(TWIHS_SLAVE_TRANSFER_EVENT_TRANSMISSION_COMPLETE, twihs0Obj.context);
                }

                TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk;
            }
        }
    }
}
