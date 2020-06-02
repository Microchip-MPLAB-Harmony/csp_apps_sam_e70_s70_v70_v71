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

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
#include "definitions.h"                // SYS function prototypes

uint8_t Mcan1MessageRAM[MCAN1_MESSAGE_RAM_CONFIG_SIZE] __attribute__((aligned (32)))__attribute__((space(data), section (".ram_nocache")));

void print_menu(void)
{
    printf(" ------------------------------ \r\n");   
    printf(" Press '1' to Transmit message \r\n");
    printf(" Press '2' to Receive message \r\n");
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint8_t user_input = 0;
    volatile uint32_t messageID = 0;
    uint8_t message[8];
    uint8_t messageLength = 0;
    uint32_t status = 0;
    
    uint8_t rx_message[8];
    uint32_t rx_messageID = 0;
    uint8_t rx_messageLength = 0;
    MCAN_MSG_RX_FRAME_ATTRIBUTE msgFrameAttr = MCAN_MSG_RX_DATA_FRAME;

    /* Initialize all modules */
    SYS_Initialize ( NULL );

    printf(" ------------------------------ \r\n");
    printf("           MCAN Demo            \r\n");
    printf(" ------------------------------ \r\n");
    
    /* Set Message RAM Configuration */
    MCAN1_MessageRAMConfigSet(Mcan1MessageRAM);

    print_menu();
    
    /* Prepare the message to send*/
    messageID = 0x469;
    messageLength = 8;
    for (uint8_t count = 8; count >=1; count--){
        message[count - 1] = count;
    }
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        scanf("%c", (char *) &user_input);
        
        switch (user_input)
        {
            case '1': 
                printf(" Transmitting Message:");
                if (MCAN1_InterruptGet(MCAN_INTERRUPT_TC_MASK))
                {
                    MCAN1_InterruptClear(MCAN_INTERRUPT_TC_MASK);
                }
                if (MCAN1_MessageTransmit(messageID,messageLength,message,MCAN_MODE_NORMAL,MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME) == true)
                {    
                    printf("Success \r\n");
                }
                else
                {
                    printf("Failed \r\n");
                }             
                break;
            case '2':
                printf(" Waiting for message: \r\n");
                while (true)
                {
                    if (MCAN1_InterruptGet(MCAN_INTERRUPT_RF0N_MASK))
                    {    
                        MCAN1_InterruptClear(MCAN_INTERRUPT_RF0N_MASK);

                        /* Check CAN Status */
                        status = MCAN1_ErrorGet();

                        if (((status & MCAN_PSR_LEC_Msk) == MCAN_ERROR_NONE) || ((status & MCAN_PSR_LEC_Msk) == MCAN_PSR_LEC_NO_CHANGE))
                        {
                            memset(rx_message, 0x00, sizeof(rx_message));
                            
                            /* Receive FIFO 0 New Message */
                            if (MCAN1_MessageReceive(&rx_messageID, &rx_messageLength, rx_message, 0, MCAN_MSG_ATTR_RX_FIFO0, &msgFrameAttr) == true)  
                            {
                                printf(" New Message Received    \r\n");
                                status = MCAN1_ErrorGet();
                                if (((status & MCAN_PSR_LEC_Msk) == MCAN_ERROR_NONE) || ((status & MCAN_PSR_LEC_Msk) == MCAN_PSR_LEC_NO_CHANGE))
                                {
                                    /* Print message to Console */
                                    uint8_t length = rx_messageLength;
                                    printf(" Message - ID : 0x%x Length : 0x%x ", (unsigned int) rx_messageID,(unsigned int) rx_messageLength);
                                    printf("Message : ");
                                    while(length)
                                    {
                                        printf("0x%x ", rx_message[rx_messageLength - length--]);
                                    }
                                    printf("\r\n");
                                    break;
                                }
                                else
                                {
                                    printf("Error in received message");
                                }
                            }
                            else
                            {
                                printf("Message Reception Failed \r");
                            }
                        }
                        else
                        {
                            printf("Error in last received message");
                        }
                    }
                }
                break;
            default:
                printf("Invalid Input \r\n");
                break;
        }
        
        print_menu();
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

