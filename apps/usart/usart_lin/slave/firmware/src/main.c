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


#define LIN_Disable()                   LIN_EN_Clear()
#define LIN_Enable()                    LIN_EN_Set()

/* LIN IDs */
#define READ_SWITCH_LED_PID             0x80
#define WRITE_LED_PID                   0xC1

#define LIN_NUM_OF_BYTES_FROM_HOST      1
#define LIN_NUM_OF_BYTES_TO_HOST        1
/*****************LIN******************/

typedef enum
{
	APP_STATE_INITIALIZE,
    APP_STATE_SUBSCRIBE,
    APP_STATE_PUBLISH,
    APP_STATE_WAIT_FOR_TRANSFER_COMPLETE,
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

uint8_t rxBuffer[10];
uint8_t txBuffer[10];
bool ledStatus = false;
volatile bool LINTransferComplete = false;
volatile bool readNotification  = false;
volatile bool LINBreakDetected = false;

void LIN_Id_Rcvd_Callback_Handler( uintptr_t context)
{
    /* Read out the PID */
    rxBuffer[0] = USART0_LIN_IdentifierRead();
    switch(rxBuffer[0])
    {            
        case READ_SWITCH_LED_PID:
            /* Client should be in PUBLISH mode now, as client has to send data requested by Master */
            appData.state = APP_STATE_PUBLISH;              
            break;

        case WRITE_LED_PID:
            /* Client should be in SUBSCRIBE mode now, as client is expecting data from Master */
            appData.state = APP_STATE_SUBSCRIBE;            
            break;
    }
        
}

void LIN_TransferComplete_Callback_Handler( uintptr_t context)
{
    LINTransferComplete = true;
}

void LIN_BreakDetect_Callback_Handler( uintptr_t context)
{
    LINBreakDetected = true;
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

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    while (1)
    {
        /* Check the application's current state. */
        switch (appData.state)
        {
            case APP_STATE_INITIALIZE:
                
                LINTransferComplete = false;
                readNotification = false;
                   
                /* EN - pin of the LIN click board to be set to proper state */
                LIN_Enable();
                
                /* Register a callback for LIN break events */
                USART0_LINBreakCallbackRegister(LIN_BreakDetect_Callback_Handler, (uintptr_t)NULL);
                  
                /* Register a callback for LIN id received events */
                USART0_LINIdCallbackRegister(LIN_Id_Rcvd_Callback_Handler, (uintptr_t)NULL);
                
                /* Register a callback for LIN data transfer complete events */
                USART0_LINTcCallbackRegister(LIN_TransferComplete_Callback_Handler, (uintptr_t)NULL);
                
                /* Register a callback for read events */
                USART0_ReadCallbackRegister(Read_Event_Callback_Handler, (uintptr_t) NULL);
                                
                /* Enable RX event notifications */
                USART0_ReadNotificationEnable(true, false);
                
                /* set a threshold value to receive a callback after every "LIN_NUM_OF_BYTES_FROM_HOST" characters are received */
                USART0_ReadThresholdSet(LIN_NUM_OF_BYTES_FROM_HOST);
                
                /* LIN response data length is set to 2 (Some default value). This will changed as per need in the later part of code.
                 *  This is only valid if Data length mode is set to DLC mode using "USART0_LIN_DataLenModeSet()" - API */
                USART0_LIN_ResponseDataLenSet(2);
                
                /* LIN identifier parity is disabled*/
                USART0_LIN_ParityEnable(false);
                
                /* LIN hardware checksum is Enabled*/
                USART0_LIN_ChecksumEnable(true);
                
                /* LIN checksum type is set to 2.0 Enhanced checksum mode. this setting is not having any impact if checksum feature is disabled */
                USART0_LIN_ChecksumTypeSet(USART_LIN_2_0_ENHANCHED_CHECKSUM);
                
                /* LIN data length mode is set to DLC. In this mode user have the flexibility to set the response data length using "USART0_LIN_ResponseDataLenEnable()" - API*/
                USART0_LIN_DataLenModeSet(USART_LIN_DATA_LEN_DLC); 
                
                /* Wait for instructions from LIN master */
                appData.state = APP_STATE_IDLE;   
                
                break;
                
            case APP_STATE_PUBLISH: 
                
                /* LIN Host has requested Switch and LED status */
                /* Switch to PUBLISH mode */
                USART0_LIN_NodeActionSet(USART_LIN_NACT_PUBLISH);
                USART0_LIN_ResponseDataLenSet(LIN_NUM_OF_BYTES_TO_HOST);
                
                /* SET requested Switch and LED status */
                txBuffer[0] = ((ledStatus << 1) | (SWITCH_Get()));
                
                USART0_Write(txBuffer, LIN_NUM_OF_BYTES_TO_HOST);
                
                /* Wait for current data transfer to complete. in this case client is sending data to HOST, so need to wait till this transfer is completed */
                appData.state = APP_STATE_WAIT_FOR_TRANSFER_COMPLETE; 
                break;
                
            case APP_STATE_SUBSCRIBE:
                
                /* waiting for data from Host */
                /* Switch to SUBSCRIBE mode*/
                USART0_LIN_NodeActionSet(USART_LIN_NACT_SUBSCRIBE);
                 
                /*Response Data length need to set to "LIN_NUM_OF_BYTES_FROM_HOST" */
                USART0_LIN_ResponseDataLenSet(LIN_NUM_OF_BYTES_FROM_HOST);
                
                appData.mode.subscribe = 1;
                
                /* Wait for current data transfer to complete. in this case HOST is sending data to Client, so need to wait till expected data received */
                appData.state = APP_STATE_WAIT_FOR_TRANSFER_COMPLETE;
                
                break;
                
            case APP_STATE_PROCESS_DATA:
                
                /* check if Receiver have received the threshold number of bytes in the ring buffer */
                if(readNotification == true)
                {
                    readNotification = false;
                  
                    /* Check the LED state requested by LIN master */
                    if (rxBuffer[0] & 0x01)
                    {
                        /* Turn on LED */
                        LED_Clear();
                        ledStatus = 1;
                    }
                    else
                    {
                        /* Turn off LED */
                        LED_Set();
                        ledStatus = 0;
                    }
                    
                    /* Wait for instructions from LIN master */
                    appData.state = APP_STATE_IDLE; 
                }
                
                break;
            
            case APP_STATE_WAIT_FOR_TRANSFER_COMPLETE:
                
                if(LINTransferComplete)
                {
                    LINTransferComplete = false;
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
                    /* Wait for instructions from LIN master */
                    appData.state = APP_STATE_IDLE;
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

