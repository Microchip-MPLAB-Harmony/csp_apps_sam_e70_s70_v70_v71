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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

/***************************************
 * Check PWM outputs on pins
 * Channel 0 PWMH - PA00
 * Channel 0 PWML - PA19
 * Channel 1 PWMH - PA02
 * Channel 1 PWML - PD25
 * Channel 2 PWMH - PC19
 * Channel 2 PWML - PD26
***************************************/

/* Duty cycle increment value */
#define DUTY_INCREMENT (10U)

/* Save PWM period */
static uint16_t period;

/* This function is called after PWM0 counter event */
void PWM0_CounterEventHandler(uint32_t status, uintptr_t context)
{
    /* duty cycle values */
    static uint16_t duty0 = 0U;
    static uint16_t duty1 = 2500U;
    static uint16_t duty2 = 5000U;

    PWM0_ChannelDutySet(PWM_CHANNEL_0, duty0);
    PWM0_ChannelDutySet(PWM_CHANNEL_1, duty1);
    PWM0_ChannelDutySet(PWM_CHANNEL_2, duty2);
    
    /* Increment duty cycle values */
    duty0 += DUTY_INCREMENT;
    duty1 += DUTY_INCREMENT;
    duty2 += DUTY_INCREMENT;
    
    if (duty0 > period)
        duty0 = 0U;
    if (duty1 > period)
        duty1 = 0U;
    if (duty2 > period)
        duty2 = 0U;
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
    
    /* Register callback function for Channel 0 counter event */
    PWM0_CallbackRegister(PWM0_CounterEventHandler, (uintptr_t)NULL);
    
    /* Read the period */
    period = PWM0_ChannelPeriodGet(PWM_CHANNEL_0);
    
    /* Start all synchronous channels by starting channel 0*/
    PWM0_ChannelsStart(PWM_CHANNEL_0_MASK);

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

