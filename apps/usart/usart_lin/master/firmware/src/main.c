/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include "definitions.h"                // SYS function prototypes

#define LIN_Disable()                                           LIN_EN_Clear()
#define LIN_Enable()                                            LIN_EN_Set()

#define LIN_NUM_OF_BYTES_TRANSFER_FROM_HOST_TO_CLIENT           1
#define LIN_NUM_OF_BYTES_TRANSFER_FROM_CLIENT_TO_HOST           1

/*****************LIN******************/

typedef enum
{
	APP_STATE_INITIALIZE,
    APP_STATE_SUBSCRIBE,
    APP_STATE_PUBLISH,
    APP_STATE_WAIT_FOR_DATA_TRANSFER_COMPLETE,
    APP_STATE_TRNASFER_COMPLETED,
    APP_STATE_PROCESS_DATA,
    APP_STATE_IDLE,

} APP_STATES;

typedef struct
{    
    uint8_t subscribe   :1;    
    uint8_t reserved    :7;
}MODE;

typedef struct
{
    volatile APP_STATES          state;   
    volatile MODE                mode;
}APP_DATA;

APP_DATA appData;

uint8_t txData[1];
uint8_t txBuffer[10];
uint8_t rxBuffer[10];

#define READ_SWITCH_LED_PID     0x80
#define WRITE_LED_PID           0xC1

uint8_t nTxBytes = 0;
volatile bool LINDataTransferComplete = false;
volatile bool LINIdTransferComplete = false;
volatile bool readNotification  = false;

void LIN_Id_Tx_Callback_Handler( uintptr_t context)
{
    LINIdTransferComplete = true; 
}

void LIN_DataTransferComplete_Callback_Handler( uintptr_t context)
{
    LINDataTransferComplete = true;
}

void Read_Event_Callback_Handler(USART_EVENT event, uintptr_t context )
{
    uint32_t nBytesAvailable = 0;
    readNotification = true;
    if (event == USART_EVENT_READ_THRESHOLD_REACHED)
    {
        /* Receiver should atleast have the thershold number of bytes in the ring buffer */
        nBytesAvailable = USART0_ReadCountGet();
        
        /* Read the thershold number of bytes here in rxBuffer */
        USART0_Read((uint8_t*)&rxBuffer, nBytesAvailable);                          
    }
}

void LIN_MasterTransmit(uint8_t pid, uint8_t* pData, uint8_t nBytes)
{
    nTxBytes = 0;       
    
    while (nBytes--)
    {
        txBuffer[nTxBytes++] = *pData++;
    }    
          
    USART0_LIN_ResponseDataLenSet(LIN_NUM_OF_BYTES_TRANSFER_FROM_HOST_TO_CLIENT);
    
    /* Transmit- PID */
    USART0_LIN_IdentifierWrite(pid);
    
    /* Transmit- DATA + CHECKSUM */ 
    USART0_Write(txBuffer, nTxBytes);
}

void LIN_MasterReceive(uint8_t pid)
{
    /*To receive data from Host , Master need to send PID */
    USART0_LIN_IdentifierWrite(pid);          
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    SYSTICK_DelayMs(100);
    
    while (1)
    {  
        /* Check the application's current state. */
        switch (appData.state)
        {
            case APP_STATE_INITIALIZE:
                
                LINDataTransferComplete = false;
                readNotification = false;
                LINIdTransferComplete = false;
                
                /* EN - pin of the LIN click board has to be set to proper state */
                LIN_Enable();
                
                /* Register a callback for LIN id Transmit events */
                USART0_LINIdCallbackRegister(LIN_Id_Tx_Callback_Handler, (uintptr_t)NULL);
                
                /* Register a callback for LIN data transfer complete events */
                USART0_LINTcCallbackRegister(LIN_DataTransferComplete_Callback_Handler, (uintptr_t)NULL);
                
                /* Register a callback for read events */
                USART0_ReadCallbackRegister(Read_Event_Callback_Handler, (uintptr_t) NULL);
                
                /* LIN response data length is set to 2 (Some default value). This will changed as per need in the later part of code.
                 *  This is only valid if Data length mode is set to DLC mode using "USART0_LIN_DataLenModeSet()" - API */
                USART0_LIN_ResponseDataLenSet(2);
                
                /* LIN identifier parity is disabled*/
                USART0_LIN_ParityEnable(false);
                
                /* LIN hardware checksum is Enabled*/
                USART0_LIN_ChecksumEnable(true);
                
                /* LIN frame slot timing is disabled*/
                USART0_LIN_FrameSlotEnable(false);
                
                /* LIN checksum type is set to 2.0 Enhanced checksum mode. this setting is not having any impact if checksum feature is disabled */
                USART0_LIN_ChecksumTypeSet(USART_LIN_2_0_ENHANCHED_CHECKSUM);
                
                /* LIN data length mode is set to DLC. In this mode user have the flexibility to set the response data length using "USART0_LIN_ResponseDataLenEnable()" - API*/
                USART0_LIN_DataLenModeSet(USART_LIN_DATA_LEN_DLC); 
                                
                /* Enable RX event notifications */
                USART0_ReadNotificationEnable(true, false);
                
                /* set a threshold value to receive a callback after every "LIN_NUM_OF_BYTES_TRANSFER_FROM_CLIENT_TO_HOST" characters are received */
                USART0_ReadThresholdSet(LIN_NUM_OF_BYTES_TRANSFER_FROM_CLIENT_TO_HOST);
                
                /* change the state to subscribe, as Host is requesting data from Client */
                appData.state = APP_STATE_SUBSCRIBE;   
                
                break;
                
            case APP_STATE_SUBSCRIBE:
                
                /* Host request data from Client after each and every 1 second */
                SYSTICK_DelayMs(1000);
                
                /* this flag needs to reset here, and will be controlled by transfer callback handler*/
                LINDataTransferComplete = false;
                
                /* LIN response data length is set to "LIN_NUM_OF_BYTES_TRANSFER_FROM_CLIENT_TO_HOST", as Host will expect that many bytes data from Client */
                USART0_LIN_ResponseDataLenSet(LIN_NUM_OF_BYTES_TRANSFER_FROM_CLIENT_TO_HOST);
                
                /* LIN Host to request client for Switch and LED status */
                /* switch to SUBSCRIBE mode, as Host is requesting data from Client */
                USART0_LIN_NodeActionSet(US_LINMR_NACT_SUBSCRIBE);  
                
                /* transmit Client id*/
                LIN_MasterReceive(READ_SWITCH_LED_PID);   
                appData.mode.subscribe = 1;      
                
                /*now need to wait till requested data received from client*/
                appData.state = APP_STATE_WAIT_FOR_DATA_TRANSFER_COMPLETE;
                
                break;
                
            case APP_STATE_PUBLISH: 
                
                /* this flag needs to reset here, and will be controlled by transfer callback handler later*/
                LINDataTransferComplete = false;
                
                /* switch to PUBLISH mode, as Host needs to send data to client */
                USART0_LIN_NodeActionSet(USART_LIN_NACT_PUBLISH);
                
                /* transmit data*/
                LIN_MasterTransmit(WRITE_LED_PID, txData, 1);

                /*now need to wait till data transfer complete*/
                appData.state = APP_STATE_WAIT_FOR_DATA_TRANSFER_COMPLETE; 
                
                break;    
            
            case APP_STATE_PROCESS_DATA:
                
                /*Check if requested num of bytes are received by Ring Buffer*/
                if(readNotification == true)
                {
                    readNotification = false;
                    
                    /* Bit position 0 contains switch status, Bit position 1 contains LED status */
                    if ((rxBuffer[0] & 0x01) == 0)
                    {
                            /* As the switch is pressed, ask LIN slave to toggle its LED */
                            txData[0] = (((rxBuffer[0] & 0x02) >> 1) ^ 0x01);            
                            
                            /* change the state to PUBLISH, as Host needs to send data to Client*/
                            appData.state = APP_STATE_PUBLISH;
                            break;
                    }
                    
                    /* change the state to subscribe, as Host needs request data from Client again*/
                    appData.state = APP_STATE_SUBSCRIBE; 
                }
                
                break;
            
            case APP_STATE_WAIT_FOR_DATA_TRANSFER_COMPLETE:
                
                /* wait for data transfer complete . When Host is in PUBLISH mode Host need to wait till specified number of data bytes are transmitted from HOST to Client, 
                 * and in SUBSCRIBE mode Host need to wait till requested number of data bytes being transmitted */
                
                if(LINDataTransferComplete)
                {
                    LINDataTransferComplete = false;
                    appData.state = APP_STATE_TRNASFER_COMPLETED;
                }
                
                break;
            
            case APP_STATE_TRNASFER_COMPLETED:
                
                if(appData.mode.subscribe == 1)
                {
                    appData.mode.subscribe = 0;
                    appData.state = APP_STATE_PROCESS_DATA;
                    
                }
                else
                {
                    /* change the state to subscribe, as Host needs to request data from Client again*/
                    appData.state = APP_STATE_SUBSCRIBE;
                }
                
                break;    
            
            case APP_STATE_IDLE: 
                
                break;
                
            default:
                break;
        }
    }
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

