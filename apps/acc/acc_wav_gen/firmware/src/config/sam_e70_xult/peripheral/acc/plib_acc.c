/*******************************************************************************
  Analog Comparator Controller (ACC) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_acc.c

  Summary:
    ACC Source File

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
#include "plib_acc.h"

// *****************************************************************************
// *****************************************************************************
// Section: ACC Implementation
// *****************************************************************************
// *****************************************************************************

void ACC_Initialize (void)
{
    uint32_t regValue = 0U;
    /*Reset ACC registers*/
    ACC_REGS->ACC_CR = ACC_CR_SWRST_Msk;

    /*Set Comparator Positive and Negative Input, Output Invert status to
      Enable/Disable, Fault Generation to Enable/Disable, Set Fault source and
      Output Edge type*/
    regValue |= ACC_MR_SELMINUS(0x2U);
    regValue |= ACC_MR_SELPLUS(0x0U);
    regValue |= ACC_MR_EDGETYP_ANY;
    regValue |= 0U;
    regValue |= 0U;
    regValue |= ACC_MR_SELFS_CE;
    regValue |= ACC_MR_ACEN_Msk;
    ACC_REGS->ACC_MR = regValue;

    /*Set Current level and Hysteresis level*/
    ACC_REGS->ACC_ACR = ACC_ACR_ISEL_LOPW | ACC_ACR_HYST(0U);


    /*Wait till output mask period gets over*/
    while ((ACC_REGS->ACC_ISR & ACC_ISR_MASK_Msk) != 0U)
    {
        /* Do nothing */
    }
}

bool ACC_StatusGet (ACC_STATUS_SOURCE status_var)
{
    return ((ACC_REGS->ACC_ISR & (uint32_t)status_var) != 0U);
}


