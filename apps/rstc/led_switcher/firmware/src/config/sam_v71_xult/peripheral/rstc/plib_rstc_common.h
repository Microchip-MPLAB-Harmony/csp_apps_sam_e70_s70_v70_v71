/*******************************************************************************
Data Type definition of Reset Controller (RSTC) Peripheral Library (PLIB).

 Company:
    Microchip Technology Inc.

 File Name:
    plib_rstc.h

 Summary:
    Data Type definition of the RSTC Peripheral Interface Plib.

 Description:
    This file defines the Data Types for the RSTC Plib.

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

#ifndef PLIB_RSTC_COMMON_H // Guards against multiple inclusion
#define PLIB_RSTC_COMMON_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/* This section lists the other files that are included in this file.
*/

#include <stddef.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* User Reset type

  Summary:
    Identifies the type of reset.

  Description:
    This enums identifies the different reset types

  Remarks:
    Refer to the specific device data sheet to determine availability.
*/
    /* First power-up reset */
#define    RSTC_GENERAL_RESET = RSTC_SR_RSTTYP_GENERAL_RST 

    /* Return from Backup Mode */
#define    RSTC_BACKUP_RESET = RSTC_SR_RSTTYP_BACKUP_RST 

    /* Watchdog fault occurred */
#define    RSTC_WATCHDOG_RESET = RSTC_SR_RSTTYP_WDT_RST 

    /* Processor reset required by the software */
#define    RSTC_SOFTWARE_RESET = RSTC_SR_RSTTYP_SOFT_RST 

    /* NRST pin detected low */
#define    RSTC_USER_RESET = RSTC_SR_RSTTYP_USER_RST

typedef uint32_t RSTC_RESET_CAUSE;

typedef void (*RSTC_CALLBACK) (uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Local: **** Do Not Use ****
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    RSTC_CALLBACK callback;
    uintptr_t     context;

} RSTC_OBJECT ;


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
// DOM-IGNORE-END

#endif // PLIB_RSTC_COMMON_H