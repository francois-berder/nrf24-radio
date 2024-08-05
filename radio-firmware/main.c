/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1829
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "config.h"
#include "mcc_generated_files/mcc.h"
#include "nrf24l01.h"
#include "serial.h"
#include "servo.h"
#include "version.h"

enum RADIO_MODE {
  RC_RECEIVER,
  RX_RADIO,
  TX_RADIO,
};
static enum RADIO_MODE mode = RC_RECEIVER;

static void enter_rc_receiver_mode(void)
{
    SERVO_Enable();
    NRF24L01_OpenReadingPipe(0, 0xE7E7E7E7E7E7E7LLU);
    NRF24L01_StartListening();
}

static void enter_rx_radio_mode(void)
{
    SERVO_Disable();
    NRF24L01_OpenReadingPipe(0, 0xE7E7E7E7E7E7E7LLU);
    NRF24L01_StartListening();
}

static void enter_tx_radio_mode(void)
{
    SERVO_Disable();
    NRF24L01_OpenWritingPipe(0xE7E7E7E7E7E7E7LLU);
    NRF24L01_StopListening();
}

static void change_mode(uint8_t new_mode)
{
    if (mode == new_mode) {
        return;
    }

    if (new_mode == RC_RECEIVER) {
        enter_rc_receiver_mode();
    } else if (new_mode == RX_RADIO) {
        enter_rx_radio_mode();
    } else if (new_mode == TX_RADIO) {
        enter_tx_radio_mode();
    } else {
        return;
    }

    mode = new_mode;
}

static void handle_serial_commands(void)
{
    struct serial_cmd_t cmd;
    while (SERIAL_Receive(&cmd)) {
        switch (cmd.id) {
        case WRITE_EEPROM:
            DATAEE_WriteByte(cmd.write_eeprom_data.addr, cmd.write_eeprom_data.data);
            break;
        case READ_EEPROM:
            cmd.id = EEPROM_DATA;
            cmd.eeprom_data.data = DATAEE_ReadByte(cmd.read_eeprom_data.addr);
            SERIAL_Send(&cmd);
            break;
        case REQ_FIRMWARE_VERSION:
            cmd.id = FIRMWARE_VERSION;
            cmd.firmware_version.major = FW_VERSION_MAJOR;
            cmd.firmware_version.minor = FW_VERSION_MINOR;
            SERIAL_Send(&cmd);
            break;
        case FIRMWARE_VERSION:
            /* Ignore */
            break;
        case START_SERVO:
            if (mode == RC_RECEIVER) {
                SERVO_Enable();
            }
            break;
        case STOP_SERVO:
            if (mode == RC_RECEIVER) {
                SERVO_Disable();
            }
            break;
        case EEPROM_DATA:
            /* Ignore */
            break;
        case RADIO_DATA:
            if (mode == TX_RADIO) {
                NRF24L01_Write(cmd.radio_data.data, sizeof(cmd.radio_data.data));
            }
            break;
        case SET_MODE:
            change_mode(cmd.set_mode.mode);
            break;
        case GET_MODE:
            cmd.id = MODE;
            cmd.mode.mode = mode;
            SERIAL_Send(&cmd);
            break;
        case MODE:
            /* Ignore */
            break;
        case RESET:
            asm("reset");
            while(1);
            break;
        }
    }
}

static void handle_radio(void)
{
    if (mode == RC_RECEIVER) {
        while (NRF24L01_AvailableData()) {
            uint8_t data[32];
            NRF24L01_Read(data, sizeof(data));
            SERVO_SetValues(data);
        }
    } else if (mode == RX_RADIO) {
        while (NRF24L01_AvailableData()) {
            uint8_t data[32];
            NRF24L01_Read(data, sizeof(data));

            struct serial_cmd_t cmd;
            cmd.id = RADIO_DATA;
            for (uint8_t i = 0; i < sizeof(data); ++i) {
                cmd.radio_data.data[i] = data[i];
            }
            SERIAL_Send(&cmd);
        }
    }
}

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    CONFIG_Initialize();
    SERVO_Initialize();

    // TODO: Use settings from CONFIG
    NRF24L01_Initialize(76, NRF24_DATARATE_1MBPS);

    mode = RC_RECEIVER;
    enter_rc_receiver_mode();

    while (1) {
        SERIAL_Process();

        handle_serial_commands();
        handle_radio();
    }
}
/**
 End of File
*/
