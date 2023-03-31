/*******************************************************************************
  Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_s25fl.c

  Summary:
    This file contains the source code for QSPI PLIB Demonstration

  Description:
    This file contains the source code for QSPI PLIB Demonstration.
    It implements the logic of Erasing, reading and writing to QSPI Flash memory

 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_s25fl.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define S25FL_STATUS_2_QUAD_ENABLE_BIT   (1 << 9)

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

static uint32_t write_index = 0;
static uint32_t sector_index = 0;

static qspi_command_xfer_t qspi_command_xfer = { 0 };
static qspi_register_xfer_t qspi_register_xfer = { 0 };
static qspi_memory_xfer_t qspi_memory_xfer = { 0 };

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* This function resets the flash by sending down the reset enable command
 * followed by the reset command. */

static APP_TRANSFER_STATUS APP_ResetFlash(void)
{
    memset((void *)&qspi_command_xfer, 0, sizeof(qspi_command_xfer_t));

    qspi_command_xfer.instruction = S25FL_CMD_FLASH_RESET_ENABLE;
    qspi_command_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_CommandWrite(&qspi_command_xfer, 0) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    qspi_command_xfer.instruction = S25FL_CMD_FLASH_RESET;
    qspi_command_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_CommandWrite(&qspi_command_xfer, 0) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    return APP_TRANSFER_COMPLETED;
}

/* Sends Write Enable command to flash */
static APP_TRANSFER_STATUS APP_WriteEnable(void)
{
    memset((void *)&qspi_command_xfer, 0, sizeof(qspi_command_xfer_t));

    qspi_command_xfer.instruction = S25FL_CMD_WRITE_ENABLE;
    qspi_command_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_CommandWrite(&qspi_command_xfer, 0) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    return APP_TRANSFER_COMPLETED;
}

/* This function reads and stores the flash id. */
static APP_TRANSFER_STATUS APP_ReadJedecId(uint32_t *jedec_id)
{
    memset((void *)&qspi_register_xfer, 0, sizeof(qspi_register_xfer_t));

    qspi_register_xfer.instruction = S25FL_CMD_JEDEC_ID_READ;
    qspi_register_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_RegisterRead(&qspi_register_xfer, jedec_id, 3) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    return APP_TRANSFER_COMPLETED;
}

/* Function to write to configuration register of the flash. */
static APP_TRANSFER_STATUS APP_WriteStatus(uint32_t *status)
{
    if (APP_TRANSFER_COMPLETED != APP_WriteEnable())
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    memset((void *)&qspi_register_xfer, 0, sizeof(qspi_register_xfer_t));

    qspi_register_xfer.instruction = S25FL_CMD_WRITE_STATUS_REG;
    qspi_register_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_RegisterWrite(&qspi_register_xfer, (uint32_t *)status, 3) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;

}

/* Function to read the status register of the flash. */
static APP_TRANSFER_STATUS APP_ReadStatus1( uint32_t *rx_data, uint32_t rx_data_length )
{
    memset((void *)&qspi_register_xfer, 0, sizeof(qspi_register_xfer_t));

    qspi_register_xfer.instruction = S25FL_CMD_READ_STATUS_1_REG;
    qspi_register_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_RegisterRead(&qspi_register_xfer, rx_data, rx_data_length) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;
}

/* Function to read the status register of the flash. */
static APP_TRANSFER_STATUS APP_ReadStatus2( uint32_t *rx_data, uint32_t rx_data_length )
{
    memset((void *)&qspi_register_xfer, 0, sizeof(qspi_register_xfer_t));

    qspi_register_xfer.instruction = S25FL_CMD_READ_STATUS_2_REG;
    qspi_register_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_RegisterRead(&qspi_register_xfer, rx_data, rx_data_length) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;
}

/* Function to read the status register of the flash. */
static APP_TRANSFER_STATUS APP_ReadStatus3( uint32_t *rx_data, uint32_t rx_data_length )
{
    memset((void *)&qspi_register_xfer, 0, sizeof(qspi_register_xfer_t));

    qspi_register_xfer.instruction = S25FL_CMD_READ_STATUS_3_REG;
    qspi_register_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_RegisterRead(&qspi_register_xfer, rx_data, rx_data_length) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;
}

/* Function to read the status register of the flash. */
static APP_TRANSFER_STATUS APP_ReadAllStatus( uint32_t *reg_status_all )
{
    uint8_t reg_status_1 = 0;
    uint8_t reg_status_2 = 0;
    uint8_t reg_status_3 = 0;

    if (APP_ReadStatus1((uint32_t *)&reg_status_1, 1) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    if (APP_ReadStatus2((uint32_t *)&reg_status_2, 1) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    if (APP_ReadStatus3((uint32_t *)&reg_status_3, 1) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    *reg_status_all = (reg_status_1 | (reg_status_2 << 8) | (reg_status_3 << 16));

    return APP_TRANSFER_COMPLETED;
}

/* Checks for any pending transfers Erase/Write */
static APP_TRANSFER_STATUS APP_TransferStatusCheck(void)
{
    uint8_t reg_status = 0;

    if (APP_ReadStatus1((uint32_t *)&reg_status, 1) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    if(reg_status & (1<<0))
        return APP_TRANSFER_BUSY;
    else
        return APP_TRANSFER_COMPLETED;
}

/* Enables the QUAD IO on the flash */
static APP_TRANSFER_STATUS APP_EnableQuadIO(void)
{
    uint32_t reg_status_all = 0;
 
    if (APP_ReadAllStatus((uint32_t *)&reg_status_all) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    reg_status_all |= S25FL_STATUS_2_QUAD_ENABLE_BIT;

    if (APP_WriteStatus(&reg_status_all) != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    while(APP_TransferStatusCheck() == APP_TRANSFER_BUSY);

    return APP_TRANSFER_COMPLETED;
}

/* Reads n Bytes of data from the flash memory */
static APP_TRANSFER_STATUS APP_MemoryRead( uint32_t *rx_data, uint32_t rx_data_length, uint32_t address )
{
    memset((void *)&qspi_memory_xfer, 0, sizeof(qspi_memory_xfer_t));

    qspi_memory_xfer.instruction = S25FL_CMD_FAST_READ;
    qspi_memory_xfer.width = SINGLE_BIT_SPI;
    qspi_memory_xfer.dummy_cycles = 8;

    if (QSPI_MemoryRead(&qspi_memory_xfer, rx_data, rx_data_length, address) == false) {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;
}

/* Reads n Bytes of data from the flash memory */
static APP_TRANSFER_STATUS APP_MemoryReadQuadIO( uint32_t *rx_data, uint32_t rx_data_length, uint32_t address )
{
    memset((void *)&qspi_memory_xfer, 0, sizeof(qspi_memory_xfer_t));

    qspi_memory_xfer.instruction = S25FL_CMD_FAST_READ_QUAD_IO;
    qspi_memory_xfer.width = QUAD_IO;
    qspi_memory_xfer.dummy_cycles = 5;
    qspi_memory_xfer.option_en = true;
    qspi_memory_xfer.option_len = OPTL_4_BIT;

    if (QSPI_MemoryRead(&qspi_memory_xfer, rx_data, rx_data_length, address) == false) {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }
    return APP_TRANSFER_COMPLETED;
}

/* Writes n Bytes of data to the flash memory */
static APP_TRANSFER_STATUS APP_MemoryWrite( uint32_t *tx_data, uint32_t tx_data_length, uint32_t address )
{
    if (APP_TRANSFER_COMPLETED != APP_WriteEnable())
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    memset((void *)&qspi_memory_xfer, 0, sizeof(qspi_memory_xfer_t));

    qspi_memory_xfer.instruction = S25FL_CMD_PAGE_PROGRAM;
    qspi_memory_xfer.width = SINGLE_BIT_SPI;

    if (QSPI_MemoryWrite(&qspi_memory_xfer, tx_data, tx_data_length, address) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    return APP_TRANSFER_COMPLETED;
}

/* Sends Erase command to flash */
static APP_TRANSFER_STATUS APP_Erase(uint8_t instruction, uint32_t address)
{
    if (APP_WriteEnable() != APP_TRANSFER_COMPLETED)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    qspi_command_xfer.instruction = instruction;
    qspi_command_xfer.width = SINGLE_BIT_SPI;
    qspi_command_xfer.addr_en = 1;

    if (QSPI_CommandWrite(&qspi_command_xfer, address) == false)
    {
        return APP_TRANSFER_ERROR_UNKNOWN;
    }

    return APP_TRANSFER_COMPLETED;
}

static APP_TRANSFER_STATUS APP_SectorErase(uint32_t address)
{
    return (APP_Erase(S25FL_CMD_SECTOR_ERASE, address));
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    uint32_t i = 0;

    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    for (i = 0; i < BUFFER_SIZE; i++)
        appData.writeBuffer[i] = i;

    SYSTICK_TimerStart();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        case APP_STATE_INIT:
        {
            appData.state = APP_STATE_RESET_FLASH;
        }

        case APP_STATE_RESET_FLASH:
        {
            if (APP_ResetFlash() != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_READ_JEDEC_ID;
            }
            break;
        }

        case APP_STATE_READ_JEDEC_ID:
        {
            if (APP_ReadJedecId(&appData.jedec_id) != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            if (appData.jedec_id != S25FL116K_JEDEC_ID)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_ERASE_FLASH;

            break;
        }

        case APP_STATE_ERASE_FLASH:
        {
            if (APP_SectorErase((MEM_START_ADDRESS + sector_index)) != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_ERASE_WAIT;

            break;
        }

        case APP_STATE_ERASE_WAIT:
        {
            if (APP_TransferStatusCheck() == APP_TRANSFER_COMPLETED)
            {
                sector_index += SECTOR_SIZE;

                if (sector_index < BUFFER_SIZE)
                {
                    appData.state = APP_STATE_ERASE_FLASH;
                }
                else
                {
                    appData.state = APP_STATE_WRITE_MEMORY;
                }
            }
            break;
        }

        case APP_STATE_WRITE_MEMORY:
        {
            if (APP_MemoryWrite((uint32_t *)&appData.writeBuffer[write_index], PAGE_SIZE, (MEM_START_ADDRESS + write_index)) != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_WRITE_WAIT;

            break;
        }

        case APP_STATE_WRITE_WAIT:
        {
            if (APP_TransferStatusCheck() == APP_TRANSFER_COMPLETED)
            {
                write_index += PAGE_SIZE;
                if (write_index < BUFFER_SIZE)
                {
                    appData.state = APP_STATE_WRITE_MEMORY;
                }
                else
                {
                    appData.state = APP_STATE_FAST_READ_MEMORY;
                }
            }
            break;
        }

        case APP_STATE_FAST_READ_MEMORY:
        {
            if (APP_MemoryRead((uint32_t *)&appData.readBuffer[0], BUFFER_SIZE, MEM_START_ADDRESS) != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_VERIFY_FAST_READ_DATA;

            break;
        }

        case APP_STATE_VERIFY_FAST_READ_DATA:
        {
            if (!memcmp(appData.writeBuffer, appData.readBuffer, BUFFER_SIZE))
            {
                appData.state = APP_STATE_ENABLE_QUAD_IO;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }

            break;
        }

        case APP_STATE_ENABLE_QUAD_IO:
        {
            if (APP_EnableQuadIO() != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                appData.state = APP_STATE_FAST_READ_QUAD_IO_MEMORY;
            }
            break;
        }

        case APP_STATE_FAST_READ_QUAD_IO_MEMORY:
        {
            memset(appData.readBuffer, 0, BUFFER_SIZE);

            if (APP_MemoryReadQuadIO((uint32_t *)&appData.readBuffer[0], BUFFER_SIZE, MEM_START_ADDRESS) != APP_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_VERIFY_FAST_READ_QUAD_IO_DATA;

            break;
        }

        case APP_STATE_VERIFY_FAST_READ_QUAD_IO_DATA:
        {
            if (!memcmp(appData.writeBuffer, appData.readBuffer, BUFFER_SIZE))
            {
                appData.state = APP_STATE_SUCCESS;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }

            break;
        }

        case APP_STATE_SUCCESS:
        {
            SYSTICK_DelayMs(1000);

            LED_TOGGLE();

            break;
        }

        case APP_STATE_ERROR:
        default:
        {
            LED_ON();
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */
