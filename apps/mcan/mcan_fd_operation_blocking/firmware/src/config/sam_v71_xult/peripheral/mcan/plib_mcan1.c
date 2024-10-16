/*******************************************************************************
  Controller Area Network (MCAN) Peripheral Library Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mcan1.c

  Summary:
    MCAN peripheral library interface.

  Description:
    This file defines the interface to the MCAN peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "interrupts.h"
#include "plib_mcan1.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************
#define MCAN_STD_ID_Msk        0x7FFU

static MCAN_OBJ mcan1Obj;

static const mcan_sidfe_registers_t mcan1StdFilter[] =
{
    {
        .MCAN_SIDFE_0 = MCAN_SIDFE_0_SFT(0UL) |
                  MCAN_SIDFE_0_SFID1(0x469UL) |
                  MCAN_SIDFE_0_SFID2(0x469UL) |
                  MCAN_SIDFE_0_SFEC(1UL)
    },
    {
        .MCAN_SIDFE_0 = MCAN_SIDFE_0_SFT(0UL) |
                  MCAN_SIDFE_0_SFID1(0x45aUL) |
                  MCAN_SIDFE_0_SFID2(0x0UL) |
                  MCAN_SIDFE_0_SFEC(7UL)
    },
};

static const mcan_xidfe_registers_t mcan1ExtFilter[] =
{
    {
        .MCAN_XIDFE_0 = MCAN_XIDFE_0_EFID1(0x100000a5UL) | MCAN_XIDFE_0_EFEC(7UL),
        .MCAN_XIDFE_1 = MCAN_XIDFE_1_EFID2(0x0UL) | MCAN_XIDFE_1_EFT(0UL),
    },
    {
        .MCAN_XIDFE_0 = MCAN_XIDFE_0_EFID1(0x10000096UL) | MCAN_XIDFE_0_EFEC(2UL),
        .MCAN_XIDFE_1 = MCAN_XIDFE_1_EFID2(0x10000096UL) | MCAN_XIDFE_1_EFT(0UL),
    },
};

// *****************************************************************************
// *****************************************************************************
// MCAN1 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void MCAN1_Initialize(void)

   Summary:
    Initializes given instance of the MCAN peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void MCAN1_Initialize(void)
{
    /* Start MCAN initialization */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk)
    {
        /* Wait for initialization complete */
    }

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

    /* Set Data Bit Timing and Prescaler Register */
    MCAN1_REGS->MCAN_DBTP = MCAN_DBTP_DTSEG2(9) | MCAN_DBTP_DTSEG1(28) | MCAN_DBTP_DBRP(0) | MCAN_DBTP_DSJW(7);

    /* Set Nominal Bit timing and Prescaler Register */
    MCAN1_REGS->MCAN_NBTP  = MCAN_NBTP_NTSEG2(39) | MCAN_NBTP_NTSEG1(118) | MCAN_NBTP_NBRP(0) | MCAN_NBTP_NSJW(39);

    /* Receive Buffer / FIFO Element Size Configuration Register */
    MCAN1_REGS->MCAN_RXESC = 0UL  | MCAN_RXESC_F0DS(7UL) | MCAN_RXESC_F1DS(7UL) | MCAN_RXESC_RBDS(7UL);
    /*lint -e{9048} PC lint incorrectly reports a missing 'U' Suffix */
    MCAN1_REGS->MCAN_NDAT1 = MCAN_NDAT1_Msk;
    /*lint -e{9048} PC lint incorrectly reports a missing 'U' Suffix */
    MCAN1_REGS->MCAN_NDAT2 = MCAN_NDAT2_Msk;

    /* Transmit Buffer/FIFO Element Size Configuration Register */
    MCAN1_REGS->MCAN_TXESC = MCAN_TXESC_TBDS(7UL);

    /* Global Filter Configuration Register */
    MCAN1_REGS->MCAN_GFC = MCAN_GFC_ANFS(2) | MCAN_GFC_ANFE(2);

    /* Extended ID AND Mask Register */
    MCAN1_REGS->MCAN_XIDAM = MCAN_XIDAM_Msk;

    /* Set the operation mode */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_FDOE_ENABLED | MCAN_CCCR_BRSE_ENABLED;


    MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
    {
        /* Wait for initialization complete */
    }

    (void) memset(&mcan1Obj.msgRAMConfig, 0x00, sizeof(MCAN_MSG_RAM_CONFIG));
}


// *****************************************************************************
/* Function:
    bool MCAN1_MessageTransmitFifo(uint8_t numberOfMessage, MCAN_TX_BUFFER *txBuffer)

   Summary:
    Transmit multiple messages into MCAN bus from Tx FIFO.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    numberOfMessage - Total number of message.
    txBuffer        - Pointer to Tx buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_MessageTransmitFifo(uint8_t numberOfMessage, MCAN_TX_BUFFER *txBuffer)
{
    uint8_t  *txFifo = NULL;
    uint8_t  *txBuf = (uint8_t *)txBuffer;
    uint32_t bufferNumber = 0U;
    uint8_t  tfqpi = 0U;
    uint8_t  count = 0U;

    if (((numberOfMessage < 1U) || (numberOfMessage > 1U)) || (txBuffer == NULL))
    {
        return false;
    }

    tfqpi = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);

    for (count = 0U; count < numberOfMessage; count++)
    {
        txFifo = (uint8_t *)((uint8_t*)mcan1Obj.msgRAMConfig.txBuffersAddress + ((uint32_t)tfqpi * MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE));

        (void) memcpy(txFifo, txBuf, MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);

        txBuf += MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE;
        bufferNumber |= (1UL << tfqpi);
        tfqpi++;
        if (tfqpi == 1U)
        {
            tfqpi = 0U;
        }
    }

    __DSB();

    /* Set Transmission request */
    MCAN1_REGS->MCAN_TXBAR = bufferNumber;

    return true;
}

// *****************************************************************************
/* Function:
    uint8_t MCAN1_TxFifoFreeLevelGet(void)

   Summary:
    Returns Tx FIFO Free Level.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Tx FIFO Free Level.
*/
uint8_t MCAN1_TxFifoFreeLevelGet(void)
{
    return (uint8_t)(MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFFL_Msk);
}

// *****************************************************************************
/* Function:
    bool MCAN1_TxBufferIsBusy(uint8_t bufferNumber)

   Summary:
    Check if Transmission request is pending for the specific Tx buffer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    true  - Transmission request is pending.
    false - Transmission request is not pending.
*/
bool MCAN1_TxBufferIsBusy(uint8_t bufferNumber)
{
    return ((MCAN1_REGS->MCAN_TXBRP & (1UL << bufferNumber)) != 0U);
}

// *****************************************************************************
/* Function:
    bool MCAN1_TxEventFifoRead(uint8_t numberOfTxEvent, MCAN_TX_EVENT_FIFO *txEventFifo)

   Summary:
    Read Tx Event FIFO for the transmitted messages.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    numberOfTxEvent - Total number of Tx Event
    txEventFifo     - Pointer to Tx Event FIFO

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_TxEventFifoRead(uint8_t numberOfTxEvent, MCAN_TX_EVENT_FIFO *txEventFifo)
{
    uint8_t txefgi     = 0U;
    uint8_t count      = 0U;
    uint8_t *txEvent   = NULL;
    uint8_t *txEvtFifo = (uint8_t *)txEventFifo;

    if (txEventFifo == NULL)
    {
        return false;
    }

    /* Read data from the Rx FIFO0 */
    txefgi = (uint8_t)((MCAN1_REGS->MCAN_TXEFS & MCAN_TXEFS_EFGI_Msk) >> MCAN_TXEFS_EFGI_Pos);
    for (count = 0U; count < numberOfTxEvent; count++)
    {
        txEvent = (uint8_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.txEventFIFOAddress + ((uint32_t)txefgi * sizeof(MCAN_TX_EVENT_FIFO)));

        (void) memcpy(txEvtFifo, txEvent, sizeof(MCAN_TX_EVENT_FIFO));

        if ((count + 1U) == numberOfTxEvent)
        {
            break;
        }
        txEvtFifo += sizeof(MCAN_TX_EVENT_FIFO);
        txefgi++;
        if (txefgi == 1U)
        {
            txefgi = 0U;
        }
    }

    /* Ack the Tx Event FIFO position */
    MCAN1_REGS->MCAN_TXEFA = MCAN_TXEFA_EFAI((uint32_t)txefgi);

    return true;
}

// *****************************************************************************
/* Function:
    uint8_t MCAN1_TxEventFifoFillLevelGet(void)

   Summary:
    Returns Tx Event FIFO Fill Level.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Tx Event FIFO Fill Level.
*/
uint8_t MCAN1_TxEventFifoFillLevelGet(void)
{
    return (uint8_t)(MCAN1_REGS->MCAN_TXEFS & MCAN_TXEFS_EFFL_Msk);
}

// *****************************************************************************
/* Function:
    bool MCAN1_MessageReceive(uint8_t bufferNumber, MCAN_RX_BUFFER *rxBuffer)

   Summary:
    Read a message from the specific Rx Buffer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    bufferNumber - Rx buffer number
    rxBuffer     - Pointer to Rx buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_MessageReceive(uint8_t bufferNumber, MCAN_RX_BUFFER *rxBuffer)
{
    uint8_t *rxBuf = NULL;
    bool message_receive_event = false;

    if((rxBuffer != NULL) && (bufferNumber < 1U))
    {
        rxBuf = (uint8_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxBuffersAddress + ((uint32_t)bufferNumber * MCAN1_RX_BUFFER_ELEMENT_SIZE));

        (void) memcpy((uint8_t *)rxBuffer, rxBuf, MCAN1_RX_BUFFER_ELEMENT_SIZE);

        /* Clear new data flag */
        MCAN1_REGS->MCAN_NDAT1 = (1UL << bufferNumber);
        message_receive_event = true;
    }
    return message_receive_event;
}

// *****************************************************************************
/* Function:
    bool MCAN1_RxBufferNumberGet(uint8_t* bufferNumber)

   Summary:
    Get Rx Buffer Number.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_RxBufferNumberGet(uint8_t* bufferNumber)
{
    bool     status = false;
    uint8_t  bufferNum = 0U;
    uint32_t newData1 = MCAN1_REGS->MCAN_NDAT1;

    if (newData1 != 0U)
    {
        for (bufferNum = 0U; bufferNum < 1U; bufferNum++)
        {
            if ((newData1 & (1UL << bufferNum)) == (1UL << bufferNum))
            {
                *bufferNumber = bufferNum;
                status = true;
                break;
            }
        }
    }

    return status;
}

// *****************************************************************************
/* Function:
    bool MCAN1_MessageReceiveFifo(MCAN_RX_FIFO_NUM rxFifoNum, uint8_t numberOfMessage, MCAN_RX_BUFFER *rxBuffer)

   Summary:
    Read messages from Rx FIFO0/FIFO1.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    rxFifoNum       - Rx FIFO number
    numberOfMessage - Total number of message
    rxBuffer        - Pointer to Rx buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_MessageReceiveFifo(MCAN_RX_FIFO_NUM rxFifoNum, uint8_t numberOfMessage, MCAN_RX_BUFFER *rxBuffer)
{
    uint8_t rxgi = 0U;
    uint8_t count = 0U;
    uint8_t *rxFifo = NULL;
    uint8_t *rxBuf = (uint8_t *)rxBuffer;
    bool status = false;

    if (rxBuffer == NULL)
    {
        return status;
    }

    switch (rxFifoNum)
    {
        case MCAN_RX_FIFO_0:
            /* Read data from the Rx FIFO0 */
            rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);
            for (count = 0U; count < numberOfMessage; count++)
            {
                rxFifo = (uint8_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO0Address + ((uint32_t)rxgi * MCAN1_RX_FIFO0_ELEMENT_SIZE));

                (void) memcpy(rxBuf, rxFifo, MCAN1_RX_FIFO0_ELEMENT_SIZE);

                if ((count + 1U) == numberOfMessage)
                {
                    break;
                }
                rxBuf += MCAN1_RX_FIFO0_ELEMENT_SIZE;
                rxgi++;
                if (rxgi == 1U)
                {
                    rxgi = 0U;
                }
            }

            /* Ack the fifo position */
            MCAN1_REGS->MCAN_RXF0A = MCAN_RXF0A_F0AI((uint32_t)rxgi);

            status = true;
            break;
        case MCAN_RX_FIFO_1:
            /* Read data from the Rx FIFO1 */
            rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1GI_Msk) >> MCAN_RXF1S_F1GI_Pos);
            for (count = 0U; count < numberOfMessage; count++)
            {
                rxFifo = (uint8_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO1Address + ((uint32_t)rxgi * MCAN1_RX_FIFO1_ELEMENT_SIZE));

                (void) memcpy(rxBuf, rxFifo, MCAN1_RX_FIFO1_ELEMENT_SIZE);

                if ((count + 1U) == numberOfMessage)
                {
                    break;
                }
                rxBuf += MCAN1_RX_FIFO1_ELEMENT_SIZE;
                rxgi++;
                if (rxgi == 1U)
                {
                    rxgi = 0U;
                }
            }
            /* Ack the fifo position */
            MCAN1_REGS->MCAN_RXF1A = MCAN_RXF1A_F1AI((uint32_t)rxgi);

            status = true;
            break;
        default:
            /* Do nothing */
            break;
    }
    return status;
}

// *****************************************************************************
/* Function:
    uint8_t MCAN1_RxFifoFillLevelGet(MCAN_RX_FIFO_NUM rxFifoNum)

   Summary:
    Returns Rx FIFO0/FIFO1 Fill Level.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Rx FIFO0/FIFO1 Fill Level.
*/
uint8_t MCAN1_RxFifoFillLevelGet(MCAN_RX_FIFO_NUM rxFifoNum)
{
    if (rxFifoNum == MCAN_RX_FIFO_0)
    {
        return (uint8_t)(MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0FL_Msk);
    }
    else
    {
        return (uint8_t)(MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1FL_Msk);
    }
}

// *****************************************************************************
/* Function:
    MCAN_ERROR MCAN1_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/
MCAN_ERROR MCAN1_ErrorGet(void)
{
    MCAN_ERROR error;
    uint32_t   errorStatus = MCAN1_REGS->MCAN_PSR;

    error = (MCAN_ERROR) ((errorStatus & MCAN_PSR_LEC_Msk) | (errorStatus & MCAN_PSR_EP_Msk) | (errorStatus & MCAN_PSR_EW_Msk)
            | (errorStatus & MCAN_PSR_BO_Msk) | (errorStatus & MCAN_PSR_DLEC_Msk) | (errorStatus & MCAN_PSR_PXE_Msk));

    if ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
    {
        MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_Msk;
        while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
        {
            /* Wait for initialization complete */
        }
    }

    return error;
}

// *****************************************************************************
/* Function:
    void MCAN1_ErrorCountGet(uint8_t *txErrorCount, uint8_t *rxErrorCount)

   Summary:
    Returns the transmit and receive error count during transfer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    txErrorCount - Transmit Error Count to be received
    rxErrorCount - Receive Error Count to be received

   Returns:
    None.
*/
void MCAN1_ErrorCountGet(uint8_t *txErrorCount, uint8_t *rxErrorCount)
{
    *txErrorCount = (uint8_t)(MCAN1_REGS->MCAN_ECR & MCAN_ECR_TEC_Msk);
    *rxErrorCount = (uint8_t)((MCAN1_REGS->MCAN_ECR & MCAN_ECR_REC_Msk) >> MCAN_ECR_REC_Pos);
}

// *****************************************************************************
/* Function:
    bool MCAN1_InterruptGet(MCAN_INTERRUPT_MASK interruptMask)

   Summary:
    Returns the Interrupt status.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    interruptMask - Interrupt source number

   Returns:
    true - Requested interrupt is occurred.
    false - Requested interrupt is not occurred.
*/
bool MCAN1_InterruptGet(MCAN_INTERRUPT_MASK interruptMask)
{
    return ((MCAN1_REGS->MCAN_IR & (uint32_t)interruptMask) != 0x0U);
}

// *****************************************************************************
/* Function:
    void MCAN1_InterruptClear(MCAN_INTERRUPT_MASK interruptMask)

   Summary:
    Clears Interrupt status.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    interruptMask - Interrupt to be cleared

   Returns:
    None
*/
void MCAN1_InterruptClear(MCAN_INTERRUPT_MASK interruptMask)
{
    MCAN1_REGS->MCAN_IR = (uint32_t)interruptMask;
}

// *****************************************************************************
/* Function:
    void MCAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)

   Summary:
    Set the Message RAM Configuration.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    msgRAMConfigBaseAddress - Pointer to application allocated buffer base address.
                              Application must allocate buffer from non-cached
                              contiguous memory and buffer size must be
                              MCAN1_MESSAGE_RAM_CONFIG_SIZE

   Returns:
    None
*/
void MCAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)
{
    uint32_t offset = 0U;
    uint32_t msgRAMConfigBaseAddr = (uint32_t)msgRAMConfigBaseAddress;

    (void) memset(msgRAMConfigBaseAddress, 0x00, MCAN1_MESSAGE_RAM_CONFIG_SIZE);

    /* Set MCAN CCCR Init for Message RAM Configuration */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk)
    {
        /* Wait for configuration complete */
    }

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

    mcan1Obj.msgRAMConfig.rxFIFO0Address = (mcan_rxf0e_registers_t *)msgRAMConfigBaseAddr;
    offset = MCAN1_RX_FIFO0_SIZE;
    /* Receive FIFO 0 Configuration Register */
    MCAN1_REGS->MCAN_RXF0C = MCAN_RXF0C_F0S(1UL) | MCAN_RXF0C_F0WM(0UL) | MCAN_RXF0C_F0OM_Msk |
            MCAN_RXF0C_F0SA(((uint32_t)mcan1Obj.msgRAMConfig.rxFIFO0Address >> 2));

    mcan1Obj.msgRAMConfig.rxFIFO1Address = (mcan_rxf1e_registers_t *)(msgRAMConfigBaseAddr + offset);
    offset += MCAN1_RX_FIFO1_SIZE;
    /* Receive FIFO 1 Configuration Register */
    MCAN1_REGS->MCAN_RXF1C = MCAN_RXF1C_F1S(1UL) | MCAN_RXF1C_F1WM(0UL) | MCAN_RXF1C_F1OM_Msk |
            MCAN_RXF1C_F1SA(((uint32_t)mcan1Obj.msgRAMConfig.rxFIFO1Address >> 2));

    mcan1Obj.msgRAMConfig.rxBuffersAddress = (mcan_rxbe_registers_t *)(msgRAMConfigBaseAddr + offset);
    offset += MCAN1_RX_BUFFER_SIZE;
    MCAN1_REGS->MCAN_RXBC = MCAN_RXBC_RBSA(((uint32_t)mcan1Obj.msgRAMConfig.rxBuffersAddress >> 2));

    mcan1Obj.msgRAMConfig.txBuffersAddress = (mcan_txbe_registers_t *)(msgRAMConfigBaseAddr + offset);
    offset += MCAN1_TX_FIFO_BUFFER_SIZE;
    /* Transmit Buffer/FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXBC = MCAN_TXBC_TFQS(1UL) |
            MCAN_TXBC_TBSA(((uint32_t)mcan1Obj.msgRAMConfig.txBuffersAddress >> 2));

    mcan1Obj.msgRAMConfig.txEventFIFOAddress =  (mcan_txefe_registers_t *)(msgRAMConfigBaseAddr + offset);
    offset += MCAN1_TX_EVENT_FIFO_SIZE;
    /* Transmit Event FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXEFC = MCAN_TXEFC_EFWM(0UL) | MCAN_TXEFC_EFS(1UL) |
            MCAN_TXEFC_EFSA(((uint32_t)mcan1Obj.msgRAMConfig.txEventFIFOAddress >> 2));

    mcan1Obj.msgRAMConfig.stdMsgIDFilterAddress = (mcan_sidfe_registers_t *)(msgRAMConfigBaseAddr + offset);
    (void) memcpy((void*)mcan1Obj.msgRAMConfig.stdMsgIDFilterAddress,
           (const void*)mcan1StdFilter,
           MCAN1_STD_MSG_ID_FILTER_SIZE);
    offset += MCAN1_STD_MSG_ID_FILTER_SIZE;
    /* Standard ID Filter Configuration Register */
    MCAN1_REGS->MCAN_SIDFC = MCAN_SIDFC_LSS(2UL) |
            MCAN_SIDFC_FLSSA(((uint32_t)mcan1Obj.msgRAMConfig.stdMsgIDFilterAddress >> 2));

    mcan1Obj.msgRAMConfig.extMsgIDFilterAddress = (mcan_xidfe_registers_t *)(msgRAMConfigBaseAddr + offset);
    (void) memcpy((void*)mcan1Obj.msgRAMConfig.extMsgIDFilterAddress,
           (const void*)mcan1ExtFilter,
           MCAN1_EXT_MSG_ID_FILTER_SIZE);
    /* Extended ID Filter Configuration Register */
    MCAN1_REGS->MCAN_XIDFC = MCAN_XIDFC_LSE(2UL) |
            MCAN_XIDFC_FLESA(((uint32_t)mcan1Obj.msgRAMConfig.extMsgIDFilterAddress >> 2));

    /* Set 16-bit MSB of mcan1 base address */
    MATRIX_REGS->CCFG_SYSIO = (MATRIX_REGS->CCFG_SYSIO & ~CCFG_SYSIO_CAN1DMABA_Msk)
                            | CCFG_SYSIO_CAN1DMABA(((uint32_t)msgRAMConfigBaseAddr >> 16));

    /* Reference offset variable once to remove warning about the variable not being used after increment */
    (void)offset;

    /* Complete Message RAM Configuration by clearing MCAN CCCR Init */
    MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
    {
        /* Wait for configuration complete */
    }
}

// *****************************************************************************
/* Function:
    bool MCAN1_StandardFilterElementSet(uint8_t filterNumber, mcan_sidfe_registers_t *stdMsgIDFilterElement)

   Summary:
    Set a standard filter element configuration.

   Precondition:
    MCAN1_Initialize and MCAN1_MessageRAMConfigSet must have been called
    for the associated MCAN instance.

   Parameters:
    filterNumber          - Standard Filter number to be configured.
    stdMsgIDFilterElement - Pointer to Standard Filter Element configuration to be set on specific filterNumber.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_StandardFilterElementSet(uint8_t filterNumber, mcan_sidfe_registers_t *stdMsgIDFilterElement)
{
    if ((filterNumber > 2U) || (stdMsgIDFilterElement == NULL))
    {
        return false;
    }
    mcan1Obj.msgRAMConfig.stdMsgIDFilterAddress[filterNumber - 1U].MCAN_SIDFE_0 = stdMsgIDFilterElement->MCAN_SIDFE_0;

    return true;
}

// *****************************************************************************
/* Function:
    bool MCAN1_StandardFilterElementGet(uint8_t filterNumber, mcan_sidfe_registers_t *stdMsgIDFilterElement)

   Summary:
    Get a standard filter element configuration.

   Precondition:
    MCAN1_Initialize and MCAN1_MessageRAMConfigSet must have been called
    for the associated MCAN instance.

   Parameters:
    filterNumber          - Standard Filter number to get filter configuration.
    stdMsgIDFilterElement - Pointer to Standard Filter Element configuration for storing filter configuration.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_StandardFilterElementGet(uint8_t filterNumber, mcan_sidfe_registers_t *stdMsgIDFilterElement)
{
    if ((filterNumber > 2U) || (stdMsgIDFilterElement == NULL))
    {
        return false;
    }
    stdMsgIDFilterElement->MCAN_SIDFE_0 = mcan1Obj.msgRAMConfig.stdMsgIDFilterAddress[filterNumber - 1U].MCAN_SIDFE_0;

    return true;
}

// *****************************************************************************
/* Function:
    bool MCAN1_ExtendedFilterElementSet(uint8_t filterNumber, mcan_xidfe_registers_t *extMsgIDFilterElement)

   Summary:
    Set a Extended filter element configuration.

   Precondition:
    MCAN1_Initialize and MCAN1_MessageRAMConfigSet must have been called
    for the associated MCAN instance.

   Parameters:
    filterNumber          - Extended Filter number to be configured.
    extMsgIDFilterElement - Pointer to Extended Filter Element configuration to be set on specific filterNumber.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_ExtendedFilterElementSet(uint8_t filterNumber, mcan_xidfe_registers_t *extMsgIDFilterElement)
{
    if ((filterNumber > 2U) || (extMsgIDFilterElement == NULL))
    {
        return false;
    }
    mcan1Obj.msgRAMConfig.extMsgIDFilterAddress[filterNumber - 1U].MCAN_XIDFE_0 = extMsgIDFilterElement->MCAN_XIDFE_0;
    mcan1Obj.msgRAMConfig.extMsgIDFilterAddress[filterNumber - 1U].MCAN_XIDFE_1 = extMsgIDFilterElement->MCAN_XIDFE_1;

    return true;
}

// *****************************************************************************
/* Function:
    bool MCAN1_ExtendedFilterElementGet(uint8_t filterNumber, mcan_xidfe_registers_t *extMsgIDFilterElement)

   Summary:
    Get a Extended filter element configuration.

   Precondition:
    MCAN1_Initialize and MCAN1_MessageRAMConfigSet must have been called
    for the associated MCAN instance.

   Parameters:
    filterNumber          - Extended Filter number to get filter configuration.
    extMsgIDFilterElement - Pointer to Extended Filter Element configuration for storing filter configuration.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_ExtendedFilterElementGet(uint8_t filterNumber, mcan_xidfe_registers_t *extMsgIDFilterElement)
{
    if ((filterNumber > 2U) || (extMsgIDFilterElement == NULL))
    {
        return false;
    }
    extMsgIDFilterElement->MCAN_XIDFE_0 = mcan1Obj.msgRAMConfig.extMsgIDFilterAddress[filterNumber - 1U].MCAN_XIDFE_0;
    extMsgIDFilterElement->MCAN_XIDFE_1 = mcan1Obj.msgRAMConfig.extMsgIDFilterAddress[filterNumber - 1U].MCAN_XIDFE_1;

    return true;
}

void MCAN1_SleepModeEnter(void)
{
    MCAN1_REGS->MCAN_CCCR |=  MCAN_CCCR_CSR_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_CSA_Msk) != MCAN_CCCR_CSA_Msk)
    {
        /* Wait for clock stop request to complete */
    }
}

void MCAN1_SleepModeExit(void)
{
    MCAN1_REGS->MCAN_CCCR &=  ~MCAN_CCCR_CSR_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_CSA_Msk) == MCAN_CCCR_CSA_Msk)
    {
        /* Wait for no clock stop */
    }
    MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_Msk;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
    {
        /* Wait for initialization complete */
    }
}

bool MCAN1_BitTimingCalculationGet(MCAN_BIT_TIMING_SETUP *setup, MCAN_BIT_TIMING *bitTiming)
{
    bool status = false;
    uint32_t numOfTimeQuanta;
    uint8_t tseg1;
    float temp1;
    float temp2;

    if ((setup != NULL) && (bitTiming != NULL))
    {
        if (setup->nominalBitTimingSet == true)
        {
            numOfTimeQuanta = MCAN1_CLOCK_FREQUENCY / (setup->nominalBitRate * ((uint32_t)setup->nominalPrescaler + 1U));
            if ((numOfTimeQuanta >= 4U) && (numOfTimeQuanta <= 385U))
            {
                if (setup->nominalSamplePoint < 50.0f)
                {
                    setup->nominalSamplePoint = 50.0f;
                }
                temp1 = (float)numOfTimeQuanta;
                temp2 = (temp1 * setup->nominalSamplePoint) / 100.0f;
                tseg1 = (uint8_t)temp2;
                bitTiming->nominalBitTiming.nominalTimeSegment2 = (uint8_t)(numOfTimeQuanta - tseg1 - 1U);
                bitTiming->nominalBitTiming.nominalTimeSegment1 = tseg1 - 2U;
                bitTiming->nominalBitTiming.nominalSJW = bitTiming->nominalBitTiming.nominalTimeSegment2;
                bitTiming->nominalBitTiming.nominalPrescaler = setup->nominalPrescaler;
                bitTiming->nominalBitTimingSet = true;
                status = true;
            }
            else
            {
                bitTiming->nominalBitTimingSet = false;
            }
        }
        if (setup->dataBitTimingSet == true)
        {
            numOfTimeQuanta = MCAN1_CLOCK_FREQUENCY / (setup->dataBitRate * ((uint32_t)setup->dataPrescaler + 1U));
            if ((numOfTimeQuanta >= 4U) && (numOfTimeQuanta <= 49U))
            {
                if (setup->dataSamplePoint < 50.0f)
                {
                    setup->dataSamplePoint = 50.0f;
                }
                temp1 = (float)numOfTimeQuanta;
                temp2 = (temp1 * setup->dataSamplePoint) / 100.0f;
                tseg1 = (uint8_t)temp2;
                bitTiming->dataBitTiming.dataTimeSegment2 = (uint8_t)(numOfTimeQuanta - tseg1 - 1U);
                bitTiming->dataBitTiming.dataTimeSegment1 = tseg1 - 2U;
                bitTiming->dataBitTiming.dataSJW = bitTiming->dataBitTiming.dataTimeSegment2;
                bitTiming->dataBitTiming.dataPrescaler = setup->dataPrescaler;
                bitTiming->dataBitTimingSet = true;
                status = true;
            }
            else
            {
                bitTiming->dataBitTimingSet = false;
                status = false;
            }
        }
    }

    return status;
}

bool MCAN1_BitTimingSet(MCAN_BIT_TIMING *bitTiming)
{
    bool status = false;
    bool nominalBitTimingSet = false;
    bool dataBitTimingSet = false;

    if ((bitTiming->nominalBitTimingSet == true)
    && (bitTiming->nominalBitTiming.nominalTimeSegment1 >= 0x1U)
    && (bitTiming->nominalBitTiming.nominalTimeSegment2 <= 0x7FU)
    && (bitTiming->nominalBitTiming.nominalPrescaler <= 0x1FFU)
    && (bitTiming->nominalBitTiming.nominalSJW <= 0x7FU))
    {
        nominalBitTimingSet = true;
    }

    if  ((bitTiming->dataBitTimingSet == true)
    &&  ((bitTiming->dataBitTiming.dataTimeSegment1 >= 0x1U) && (bitTiming->dataBitTiming.dataTimeSegment1 <= 0x1FU))
    &&  (bitTiming->dataBitTiming.dataTimeSegment2 <= 0xFU)
    &&  (bitTiming->dataBitTiming.dataPrescaler <= 0x1FU)
    &&  (bitTiming->dataBitTiming.dataSJW <= 0xFU))
    {
        dataBitTimingSet = true;
    }

    if ((nominalBitTimingSet == true) || (dataBitTimingSet == true))
    {
        /* Start MCAN initialization */
        MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_Msk;
        while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk)
        {
            /* Wait for initialization complete */
        }

        /* Set CCE to unlock the configuration registers */
        MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

        if (dataBitTimingSet == true)
        {
            /* Set Data Bit Timing and Prescaler Register */
            MCAN1_REGS->MCAN_DBTP = MCAN_DBTP_DTSEG2(bitTiming->dataBitTiming.dataTimeSegment2) | MCAN_DBTP_DTSEG1(bitTiming->dataBitTiming.dataTimeSegment1) | MCAN_DBTP_DBRP(bitTiming->dataBitTiming.dataPrescaler) | MCAN_DBTP_DSJW(bitTiming->dataBitTiming.dataSJW);

        }

        if (nominalBitTimingSet == true)
        {
            /* Set Nominal Bit timing and Prescaler Register */
            MCAN1_REGS->MCAN_NBTP  = MCAN_NBTP_NTSEG2(bitTiming->nominalBitTiming.nominalTimeSegment2) | MCAN_NBTP_NTSEG1(bitTiming->nominalBitTiming.nominalTimeSegment1) | MCAN_NBTP_NBRP(bitTiming->nominalBitTiming.nominalPrescaler) | MCAN_NBTP_NSJW(bitTiming->nominalBitTiming.nominalSJW);
        }

        /* Set the operation mode */
        MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_FDOE_ENABLED | MCAN_CCCR_BRSE_ENABLED;


        MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_Msk;
        while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
        {
            /* Wait for initialization complete */
        }
        status = true;
    }
    return status;
}

/*******************************************************************************
 End of File
*/
