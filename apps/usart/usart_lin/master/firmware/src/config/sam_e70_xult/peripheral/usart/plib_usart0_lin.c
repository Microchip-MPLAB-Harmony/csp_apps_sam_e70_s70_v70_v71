/*******************************************************************************
  USART0 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_usart0_LIN.c

  Summary:
    USART0 PLIB Implementation File in LIN mode

  Description:
    None

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

#include "device.h"
#include "plib_usart0_lin.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: USART0 Implementation
// *****************************************************************************
// *****************************************************************************

#define USART0_READ_BUFFER_SIZE      128U
#define USART0_READ_BUFFER_SIZE_9BIT        (128U >> 1)
/* Disable Read, Overrun, Parity and Framing error interrupts */
#define USART0_RX_INT_DISABLE()      USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk |  US_IDR_LIN_OVRE_Msk | US_IDR_LIN_FRAME_Msk | US_IDR_LIN_PARE_Msk | US_IDR_LIN_TIMEOUT_Msk \
																							| US_IDR_LIN_LINBE_Msk | US_IDR_LIN_LINISFE_Msk | US_IDR_LIN_LINIPE_Msk \
																							| US_IDR_LIN_LINCE_Msk | US_IDR_LIN_LINSNRE_Msk | US_IDR_LIN_LINSTE_Msk | US_IDR_LIN_LINHTE_Msk);
/* Enable Read, Overrun, Parity and Framing error interrupts */
#define USART0_RX_INT_ENABLE()       USART0_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_LIN_OVRE_Msk | US_IER_LIN_FRAME_Msk | US_IER_LIN_PARE_Msk | US_IER_LIN_TIMEOUT_Msk \
																							| US_IER_LIN_LINBE_Msk | US_IER_LIN_LINISFE_Msk | US_IER_LIN_LINIPE_Msk \
																							| US_IER_LIN_LINCE_Msk | US_IER_LIN_LINSNRE_Msk | US_IER_LIN_LINSTE_Msk | US_IER_LIN_LINHTE_Msk);

#define USART0_WRITE_BUFFER_SIZE     128U
#define USART0_WRITE_BUFFER_SIZE_9BIT       (128U >> 1)
#define USART0_TX_INT_DISABLE()      USART0_REGS->US_IDR = US_IDR_USART_TXRDY_Msk;
#define USART0_TX_INT_ENABLE()       USART0_REGS->US_IER = US_IER_USART_TXRDY_Msk;

/* LIN related Interrupts */
#define USART0_LIN_LINTC_INT_DISABLE()      USART0_REGS->US_IDR = US_IDR_LIN_LINTC_Msk;
#define USART0_LIN_LINTC_INT_ENABLE()       USART0_REGS->US_IER = US_IER_LIN_LINTC_Msk;

#define USART0_LIN_LINID_INT_DISABLE()      USART0_REGS->US_IDR = US_IDR_LIN_LINID_Msk;
#define USART0_LIN_LINID_INT_ENABLE()       USART0_REGS->US_IER = US_IER_LIN_LINID_Msk;

#define USART0_LIN_LINBK_INT_DISABLE()      USART0_REGS->US_IDR = US_IDR_LIN_LINBK_Msk;
#define USART0_LIN_LINBK_INT_ENABLE()       USART0_REGS->US_IER = US_IER_LIN_LINBK_Msk;

volatile static uint8_t USART0_ReadBuffer[USART0_READ_BUFFER_SIZE];
volatile static USART_RING_BUFFER_OBJECT usart0Obj;
volatile static uint8_t USART0_WriteBuffer[USART0_WRITE_BUFFER_SIZE];
volatile static USART_LIN_CALLBACK_OBJECT usart0LinObj;

void USART0_Initialize( void )
{
    /* Reset USART0 */
    USART0_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk);

    /* Enable USART0 */
    USART0_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);

    /* Configure USART0 mode */
    USART0_REGS->US_MR = (US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | US_MR_USART_OVER(0));

    USART0_REGS->US_MR |= US_MR_USART_MODE_LIN_MASTER;
 USART0_REGS->US_LINMR =
US_LINMR_CHKTYP(0x0U) | US_LINMR_DLM(0x0U) |
US_LINMR_DLC(8U) | US_LINMR_WKUPTYP(0x0U);    

    /* Configure USART0 Baud Rate */
    USART0_REGS->US_BRGR = US_BRGR_CD(721U);

    /* Initialize instance object */
    usart0Obj.rdCallback = NULL;
    usart0Obj.rdInIndex = 0;
    usart0Obj.rdOutIndex = 0;
    usart0Obj.isRdNotificationEnabled = false;
    usart0Obj.isRdNotifyPersistently = false;
    usart0Obj.rdThreshold = 0;
    usart0Obj.errorStatus = USART_ERROR_NONE;
    usart0Obj.wrCallback = NULL;
    usart0Obj.wrInIndex = 0;
    usart0Obj.wrOutIndex = 0;
    usart0Obj.isWrNotificationEnabled = false;
    usart0Obj.isWrNotifyPersistently = false;
    usart0Obj.wrThreshold = 0;

    if((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
    {
        usart0Obj.rdBufferSize = USART0_READ_BUFFER_SIZE_9BIT;
        usart0Obj.wrBufferSize = USART0_WRITE_BUFFER_SIZE_9BIT;
    }
    else
    {
        usart0Obj.rdBufferSize = USART0_READ_BUFFER_SIZE;
        usart0Obj.wrBufferSize = USART0_WRITE_BUFFER_SIZE;
    }

    /* Enable Read, Overrun, Parity and Framing error interrupts */
    USART0_RX_INT_ENABLE();
	
	/* Enable LIN ID interrupt */
	USART0_LIN_LINID_INT_ENABLE();
	
	/* Enable LIN Transfer Complete interrupt */
    USART0_LIN_LINTC_INT_ENABLE();
	
	/* Enable LIN Break interrupt */
    USART0_LIN_LINBK_INT_ENABLE();
}

bool USART0_SerialSetup( USART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    uint32_t baud;
    uint32_t brgVal = 0;
    uint32_t overSampVal = 0;
    uint32_t usartMode;
    bool status = (setup != NULL);

    /*Valid pointer */
    if(status)
    {
        baud = setup->baudRate;
        if(srcClkFreq == 0U)
        {
            srcClkFreq = USART0_FrequencyGet();
        }

        /* Calculate BRG value */
        if (srcClkFreq >= (16U * baud))
        {
            brgVal = (srcClkFreq / (16U * baud));
        }
        else if (srcClkFreq >= (8U * baud))
        {
            brgVal = (srcClkFreq / (8U * baud));
            overSampVal = US_MR_USART_OVER(1U);
        }
        else
        {
           /* Invalid clock */
           status = false;
        }

        status = status && (brgVal <= 65535U);
        /* Target baud is achievable */
        if(status)
        {
            /* Configure USART0 mode */
            usartMode = USART0_REGS->US_MR;
            usartMode &= ~(US_MR_USART_CHRL_Msk | US_MR_USART_MODE9_Msk | US_MR_USART_PAR_Msk | US_MR_USART_NBSTOP_Msk | US_MR_USART_OVER_Msk);
            USART0_REGS->US_MR = usartMode | ((uint32_t)setup->dataWidth | (uint32_t)setup->parity | (uint32_t)setup->stopBits | (uint32_t)overSampVal);

            /* Configure USART0 Baud Rate */
            USART0_REGS->US_BRGR = US_BRGR_CD(brgVal);

            if((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
            {
                usart0Obj.rdBufferSize = USART0_READ_BUFFER_SIZE_9BIT;
                usart0Obj.wrBufferSize = USART0_WRITE_BUFFER_SIZE_9BIT;
            }
            else
            {
                usart0Obj.rdBufferSize = USART0_READ_BUFFER_SIZE;
                usart0Obj.wrBufferSize = USART0_WRITE_BUFFER_SIZE;
            }

        }
    }
    return status;
}

static void USART0_ErrorClear( void )
{
    uint32_t dummyData = 0U;

    USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( US_CSR_USART_RXRDY_Msk == (USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk))
    {
        dummyData = (USART0_REGS->US_RHR & US_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

USART_ERROR USART0_ErrorGet( void )
{
    USART_ERROR errors = usart0Obj.errorStatus;

    usart0Obj.errorStatus = USART_ERROR_NONE;

    /* All errors are cleared, but send the previous error state */
    return errors;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool USART0_RxPushByte(uint16_t rdByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;
    uint32_t rdInIdx;

    tempInIndex = usart0Obj.rdInIndex + 1U;

    if (tempInIndex >= usart0Obj.rdBufferSize)
    {
        tempInIndex = 0U;
    }

    if (tempInIndex == usart0Obj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(usart0Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = usart0Obj.rdContext;

            usart0Obj.rdCallback(USART_EVENT_READ_BUFFER_FULL, rdContext);

            /* Read the indices again in case application has freed up space in RX ring buffer */
            tempInIndex = usart0Obj.rdInIndex + 1U;

            if (tempInIndex >= usart0Obj.rdBufferSize)
            {
                tempInIndex = 0U;
            }
        }
    }

    if (tempInIndex != usart0Obj.rdOutIndex)
    {
        if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
        {
            rdInIdx = usart0Obj.rdInIndex << 1U;
            USART0_ReadBuffer[rdInIdx] = (uint8_t)rdByte;
            USART0_ReadBuffer[rdInIdx + 1U] = (uint8_t)(rdByte >> 8U);
        }
        else
        {
            uint32_t rdInIndex = usart0Obj.rdInIndex;

            USART0_ReadBuffer[rdInIndex] = (uint8_t)rdByte;
        }

        usart0Obj.rdInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void USART0_ReadNotificationSend(void)
{
    uint32_t nUnreadBytesAvailable;

    if (usart0Obj.isRdNotificationEnabled == true)
    {
        nUnreadBytesAvailable = USART0_ReadCountGet();

        if(usart0Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = usart0Obj.rdContext;

            if (usart0Obj.isRdNotifyPersistently == true)
            {
                if (nUnreadBytesAvailable >= usart0Obj.rdThreshold)
                {
                    usart0Obj.rdCallback(USART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
            else
            {
                if (nUnreadBytesAvailable == usart0Obj.rdThreshold)
                {
                    usart0Obj.rdCallback(USART_EVENT_READ_THRESHOLD_REACHED, rdContext);
                }
            }
        }
    }
}

size_t USART0_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0;

    /* Take a snapshot of indices to avoid creation of critical section */
    uint32_t rdOutIndex = usart0Obj.rdOutIndex;
    uint32_t rdInIndex = usart0Obj.rdInIndex;
    uint32_t rdOut16Idx;
    uint32_t nBytesRead16Idx;

    while (nBytesRead < size)
    {
        if (rdOutIndex != rdInIndex)
        {
            if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
            {
                rdOut16Idx = rdOutIndex << 1U;
                nBytesRead16Idx = nBytesRead << 1U;

                pRdBuffer[nBytesRead16Idx] = USART0_ReadBuffer[rdOut16Idx];
                pRdBuffer[nBytesRead16Idx + 1U] = USART0_ReadBuffer[rdOut16Idx + 1U];
            }
            else
            {
                pRdBuffer[nBytesRead] = USART0_ReadBuffer[rdOutIndex];
            }

            nBytesRead++;
            rdOutIndex++;

            if (rdOutIndex >= usart0Obj.rdBufferSize)
            {
                rdOutIndex = 0;
            }
        }
        else
        {
            /* No more data available in the RX buffer */
            break;
        }
    }

    usart0Obj.rdOutIndex = rdOutIndex;

    return nBytesRead;
}

size_t USART0_ReadCountGet(void)
{
    size_t nUnreadBytesAvailable;

    /* Take a snapshot of indices to avoid creation of critical section */
    uint32_t rdOutIndex = usart0Obj.rdOutIndex;
    uint32_t rdInIndex = usart0Obj.rdInIndex;

    if ( rdInIndex >=  rdOutIndex)
    {
        nUnreadBytesAvailable =  rdInIndex - rdOutIndex;
    }
    else
    {
        nUnreadBytesAvailable =  (usart0Obj.rdBufferSize -  rdOutIndex) + rdInIndex;
    }

    return nUnreadBytesAvailable;
}

size_t USART0_ReadFreeBufferCountGet(void)
{
    return (usart0Obj.rdBufferSize - 1U) - USART0_ReadCountGet();
}

size_t USART0_ReadBufferSizeGet(void)
{
    return (usart0Obj.rdBufferSize - 1U);
}

bool USART0_ReadNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = usart0Obj.isRdNotificationEnabled;

    usart0Obj.isRdNotificationEnabled = isEnabled;

    usart0Obj.isRdNotifyPersistently = isPersistent;

    return previousStatus;
}

void USART0_ReadThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        usart0Obj.rdThreshold = nBytesThreshold;
    }
}

void USART0_ReadCallbackRegister( USART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    usart0Obj.rdCallback = callback;

    usart0Obj.rdContext = context;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static bool USART0_TxPullByte(void* pWrData)
{
    uint8_t* pWrByte = (uint8_t*)pWrData;
    uint32_t wrOutIndex = usart0Obj.wrOutIndex;
    uint32_t wrInIndex = usart0Obj.wrInIndex;
    uint32_t wrOut16Idx;
    bool isSuccess = false;

    if (wrOutIndex != wrInIndex)
    {
        if((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
        {
            wrOut16Idx = wrOutIndex << 1U;
            pWrByte[0] = USART0_WriteBuffer[wrOut16Idx];
            pWrByte[1] = USART0_WriteBuffer[wrOut16Idx + 1U];
        }
        else
        {
            *pWrByte = USART0_WriteBuffer[wrOutIndex];
        }

        wrOutIndex++;
        if (wrOutIndex >= usart0Obj.wrBufferSize)
        {
            wrOutIndex = 0U;
        }

        usart0Obj.wrOutIndex = wrOutIndex;

        isSuccess = true;
    }

    return isSuccess;
}

static inline bool USART0_TxPushByte(uint16_t wrByte)
{
    uint32_t tempInIndex;
    uint32_t wrOutIndex = usart0Obj.wrOutIndex;
    uint32_t wrInIndex = usart0Obj.wrInIndex;
    uint32_t wrIn16Idx;
    bool isSuccess = false;

    tempInIndex = wrInIndex + 1U;

    if (tempInIndex >= usart0Obj.wrBufferSize)
    {
        tempInIndex = 0U;
    }
    if (tempInIndex != wrOutIndex)
    {
        if((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
        {
            wrIn16Idx = wrInIndex << 1U;
            USART0_WriteBuffer[wrIn16Idx] = (uint8_t)wrByte;
            USART0_WriteBuffer[wrIn16Idx + 1U] = (uint8_t)(wrByte >> 8U);
        }
        else
        {
            USART0_WriteBuffer[wrInIndex] = (uint8_t)wrByte;
        }

        usart0Obj.wrInIndex = tempInIndex;

        isSuccess = true;
    }
    else
    {
        /* Queue is full. Report Error. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static void USART0_WriteNotificationSend(void)
{
    uint32_t nFreeWrBufferCount;

    if (usart0Obj.isWrNotificationEnabled == true)
    {
        nFreeWrBufferCount = USART0_WriteFreeBufferCountGet();

        if(usart0Obj.wrCallback != NULL)
        {
            uintptr_t wrContext = usart0Obj.wrContext;

            if (usart0Obj.isWrNotifyPersistently == true)
            {
                if (nFreeWrBufferCount >= usart0Obj.wrThreshold)
                {
                    usart0Obj.wrCallback(USART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
            else
            {
                if (nFreeWrBufferCount == usart0Obj.wrThreshold)
                {
                    usart0Obj.wrCallback(USART_EVENT_WRITE_THRESHOLD_REACHED, wrContext);
                }
            }
        }
    }
}

static size_t USART0_WritePendingBytesGet(void)
{
    size_t nPendingTxBytes;

    /* Take a snapshot of indices to avoid creation of critical section */
    uint32_t wrOutIndex = usart0Obj.wrOutIndex;
    uint32_t wrInIndex = usart0Obj.wrInIndex;

    if ( wrInIndex >= wrOutIndex)
    {
        nPendingTxBytes =  wrInIndex -  wrOutIndex;
    }
    else
    {
        nPendingTxBytes =  (usart0Obj.wrBufferSize -  wrOutIndex) + wrInIndex;
    }

    return nPendingTxBytes;
}

size_t USART0_WriteCountGet(void)
{
    size_t nPendingTxBytes;

    nPendingTxBytes = USART0_WritePendingBytesGet();

    return nPendingTxBytes;
}

size_t USART0_Write(uint8_t* pWrBuffer, const size_t size )
{
    size_t nBytesWritten  = 0U;
    uint16_t halfWordData = 0U;

    while (nBytesWritten < size)
    {
        if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
        {
            halfWordData = pWrBuffer[(2U * nBytesWritten) + 1U];
            halfWordData <<= 8U;
            halfWordData |= pWrBuffer[(2U * nBytesWritten)];
            if (USART0_TxPushByte(halfWordData) == true)
            {
                nBytesWritten++;
            }
            else
            {
                /* Queue is full, exit the loop */
                break;
            }
        }
        else
        {
            if (USART0_TxPushByte(pWrBuffer[nBytesWritten]) == true)
            {
                nBytesWritten++;
            }
            else
            {
                /* Queue is full, exit the loop */
                break;
            }
        }
    }

    /* Check if any data is pending for transmission */
    if (USART0_WritePendingBytesGet() > 0U)
    {
        /* Enable TX interrupt as data is pending for transmission */
        USART0_TX_INT_ENABLE();
    }

    return nBytesWritten;
}

size_t USART0_WriteFreeBufferCountGet(void)
{
    return (usart0Obj.wrBufferSize - 1U) - USART0_WriteCountGet();
}

size_t USART0_WriteBufferSizeGet(void)
{
    return (usart0Obj.wrBufferSize - 1U);
}

bool USART0_TransmitComplete(void)
{
    return ((USART0_REGS->US_CSR & US_CSR_USART_TXEMPTY_Msk) != 0U);
}

bool USART0_WriteNotificationEnable(bool isEnabled, bool isPersistent)
{
    bool previousStatus = usart0Obj.isWrNotificationEnabled;

    usart0Obj.isWrNotificationEnabled = isEnabled;

    usart0Obj.isWrNotifyPersistently = isPersistent;

    return previousStatus;
}

void USART0_WriteThresholdSet(uint32_t nBytesThreshold)
{
    if (nBytesThreshold > 0U)
    {
        usart0Obj.wrThreshold = nBytesThreshold;
    }
}

void USART0_WriteCallbackRegister( USART_RING_BUFFER_CALLBACK callback, uintptr_t context)
{
    usart0Obj.wrCallback = callback;
    usart0Obj.wrContext = context;
}

static void __attribute__((used)) USART0_ISR_RX_Handler( void )
{
    uint32_t rdData = 0;

    /* Keep reading until there is a character availabe in the RX FIFO */
    while ((USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) != 0U)
    {
        rdData = USART0_REGS->US_RHR & US_RHR_RXCHR_Msk;

        if (USART0_RxPushByte((uint16_t)rdData ) == true)
        {
            USART0_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }
}

static void __attribute__((used)) USART0_ISR_TX_Handler( void )
{
    uint16_t wrByte = 0U;

    /* Keep writing to the TX FIFO as long as there is space */
    while ((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U)
    {
        if (USART0_TxPullByte(&wrByte) == true)
        {
            if ((USART0_REGS->US_MR & US_MR_USART_MODE9_Msk) != 0U)
            {
                USART0_REGS->US_THR = ((uint32_t)wrByte & US_THR_TXCHR_Msk);
            }
            else
            {
                USART0_REGS->US_THR = (uint8_t)wrByte;
            }

            /* Send notification */
            USART0_WriteNotificationSend();
        }
        else
        {
            /* Nothing to transmit. Disable the data register empty interrupt. */
            USART0_TX_INT_DISABLE();
            break;
        }
    }
}

void __attribute__((used)) USART0_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (USART0_REGS->US_CSR & (US_CSR_LIN_OVRE_Msk | US_CSR_LIN_FRAME_Msk | US_CSR_LIN_PARE_Msk | US_CSR_LIN_TIMEOUT_Msk
																	| US_CSR_LIN_LINBE_Msk | US_CSR_LIN_LINISFE_Msk | US_CSR_LIN_LINIPE_Msk
																	| US_CSR_LIN_LINCE_Msk | US_CSR_LIN_LINSNRE_Msk | US_CSR_LIN_LINSTE_Msk | US_CSR_LIN_LINHTE_Msk));

    if(errorStatus != 0U)
    {
        /* Save the error so that it can be reported when application calls the USART0_ErrorGet() API */
        usart0Obj.errorStatus = (USART_ERROR)errorStatus;

        /* Clear the error flags and flush out the error bytes */
        USART0_ErrorClear();

        /* USART errors are normally associated with the receiver, hence calling receiver callback */
        if( usart0Obj.rdCallback != NULL)
        {
            uintptr_t rdContext = usart0Obj.rdContext;

            usart0Obj.rdCallback(USART_EVENT_READ_ERROR, rdContext);
        }
    }

    /* Receiver status */
    if((USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) != 0U)
    {
        USART0_ISR_RX_Handler();
    }

    /* Transmitter status */
    if((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U)
    {
        USART0_ISR_TX_Handler();
    }
	/* LIN Break Detected */
    if((USART0_REGS->US_CSR & US_CSR_LIN_LINBK_Msk) != 0U)
    {
        USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;
       
        if( usart0LinObj.breakCallback != NULL)
        {
            uintptr_t breakContext = usart0LinObj.breakContext;
            usart0LinObj.breakCallback(breakContext);
        }
    }
    
    /* LIN ID Receive */
    if((USART0_REGS->US_CSR & US_CSR_LIN_LINID_Msk) != 0U)
    {
        USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;
       
        if( usart0LinObj.idCallback != NULL)
        {
            uintptr_t idContext = usart0LinObj.idContext;
            usart0LinObj.idCallback(idContext);
        }
    }
	
    /* LIN Transfer Complete */
    if((USART0_REGS->US_CSR & US_CSR_LIN_LINTC_Msk) != 0U)
    {
        USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;
       
        if( usart0LinObj.tranferCallback != NULL)
        {
            uintptr_t transferContext = usart0LinObj.tranferContext;
            usart0LinObj.tranferCallback(transferContext);
        }
    }
}



void USART0_LIN_NodeActionSet( USART_LIN_NACT action )
{
    USART0_REGS->US_LINMR &= ~(US_LINMR_NACT_Msk);
    USART0_REGS->US_LINMR |= US_LINMR_NACT(action);
}

bool USART0_LIN_IdentifierWrite( uint8_t id)
{
	bool status = false; 
	USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;
    
    if((USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk) != 0U)
    {
        USART0_REGS->US_LINIR = id;
		status = true;
    }
	
	return status;
}

uint8_t USART0_LIN_IdentifierRead( void)
{
    return (uint8_t)(USART0_REGS->US_LINIR);
}

void USART0_LIN_ParityEnable(bool parityEnable)
{
    if(parityEnable == true)
    {
        USART0_REGS->US_LINMR &= ~US_LINMR_PARDIS_Msk;
    }
    else
    {
        USART0_REGS->US_LINMR |= US_LINMR_PARDIS_Msk;
    }
}

void USART0_LIN_ChecksumEnable(bool checksumEnable)
{
    if(checksumEnable == true)
    {
        USART0_REGS->US_LINMR &= ~US_LINMR_CHKDIS_Msk;
    }
    else
    {
        USART0_REGS->US_LINMR |= US_LINMR_CHKDIS_Msk;
    }
}

void USART0_LIN_ChecksumTypeSet(USART_LIN_CHECKSUM_TYPE checksumType)
{
	USART0_REGS->US_LINMR &= ~US_LINMR_CHKTYP_Msk;
	USART0_REGS->US_LINMR |= (uint32_t)checksumType;
}

void USART0_LIN_FrameSlotEnable(bool frameSlotEnable)
{
    if(frameSlotEnable == true)
    {
        USART0_REGS->US_LINMR &= ~US_LINMR_FSDIS_Msk;
    }
    else
    {
        USART0_REGS->US_LINMR |= US_LINMR_FSDIS_Msk;
    }
}

void USART0_LIN_DataLenModeSet(USART_LIN_DATA_LEN dataLenMode)
{
    USART0_REGS->US_LINMR &= ~US_LINMR_DLM_Msk;
    USART0_REGS->US_LINMR |= (uint32_t)dataLenMode;    
}

void USART0_LIN_ResponseDataLenSet(uint8_t len)
{
    USART0_REGS->US_LINMR &= ~US_LINMR_DLC_Msk;
    USART0_REGS->US_LINMR |= US_LINMR_DLC((uint32_t)len-1U);
}

uint8_t USART0_LIN_TransferComplete(void)
{
	return (uint8_t)((USART0_REGS->US_CSR & US_CSR_LIN_LINTC_Msk) > 0U);
}

void USART0_LINIdCallbackRegister( USART_LIN_CALLBACK callback, uintptr_t context)
{
    usart0LinObj.idCallback = callback;
    usart0LinObj.idContext = context;
}

void USART0_LINTcCallbackRegister( USART_LIN_CALLBACK callback, uintptr_t context)
{
    usart0LinObj.tranferCallback = callback;
    usart0LinObj.tranferContext = context;
}

void USART0_LINBreakCallbackRegister( USART_LIN_CALLBACK callback, uintptr_t context)
{
    usart0LinObj.breakCallback = callback;
    usart0LinObj.breakContext = context;
}

