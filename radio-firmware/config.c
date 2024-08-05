#include "config.h"
#include "mcc_generated_files/mcc.h"
#include "nrf24l01.h"
#include "servo.h"

// EEPROM configuration map
#define CONFIG_MAGIC_ADDR   (0x00)
#define RADIO_CHANNEL_ADDR (0x01)
#define RADIO_DATARATE_ADDR (0x02)
#define RADIO_PAYLOAD_SIZE (0x03)
#define RADIO_PIPE_ADDRESS (0x04)
#define SERVO_POSITION (0x0A)
#define SERVO_OFFSET (0x0E)

// Default values
#define CONFIG_MAGIC_VALUE (0xDE)
#define DEFAULT_RADIO_CHANNEL (76)
#define DEFAULT_RADIO_DATARATE (NRF24_DATARATE_1MBPS)
#define DEFAULT_RADIO_PAYLOAD_SIZE (32)
#define DEFAULT_RADIO_PIPE_ADDRESS (1)
#define DEFAULT_SERVO_POSITION (128)
#define DEFAULT_SERVO_OFFSET (64)

void CONFIG_Initialize(void)
{
    uint8_t magic = DATAEE_ReadByte(CONFIG_MAGIC_ADDR);
    if (magic != CONFIG_MAGIC_VALUE) {
        CONFIG_SetRadioChannel(DEFAULT_RADIO_CHANNEL);
        CONFIG_SetRadioDataRate(DEFAULT_RADIO_DATARATE);
        CONFIG_SetPayloadSize(DEFAULT_RADIO_PAYLOAD_SIZE);
        CONFIG_SetPipeAddress(DEFAULT_RADIO_PIPE_ADDRESS);
        for (uint8_t i = 0; i < SERVO_COUNT; ++i) {
            CONFIG_SetServoPosition(i, DEFAULT_SERVO_POSITION);
            CONFIG_SetServoOffset(i, DEFAULT_SERVO_OFFSET);
        }
        DATAEE_WriteByte(CONFIG_MAGIC_ADDR, CONFIG_MAGIC_VALUE);
    }
}

void CONFIG_SetRadioChannel(uint8_t channel)
{
    DATAEE_WriteByte(RADIO_CHANNEL_ADDR, channel);
}

uint8_t CONFIG_GetRadioChannel(void)
{
   return DATAEE_ReadByte(RADIO_CHANNEL_ADDR); 
}

void CONFIG_SetRadioDataRate(enum NRF24_DataRate datarate)
{
    DATAEE_WriteByte(RADIO_DATARATE_ADDR, datarate);
}

enum NRF24_DataRate CONFIG_GetRadioDataRate(void)
{
    return DATAEE_ReadByte(RADIO_DATARATE_ADDR);
}

void CONFIG_SetPayloadSize(uint8_t payload_size)
{
    DATAEE_WriteByte(RADIO_PAYLOAD_SIZE, payload_size);
}

uint8_t CONFIG_GetPayloadSize(void)
{
    return DATAEE_ReadByte(RADIO_PAYLOAD_SIZE);
}

void CONFIG_SetPipeAddress(uint64_t address)
{
    uint8_t *ptr = (uint8_t *)&address;
    uint8_t i;
    for (i = 0; i < 6; ++i) {
        DATAEE_WriteByte(RADIO_PIPE_ADDRESS + i, ptr[i]);
    }
}

uint64_t CONFIG_GetPipeAddress(void)
{
    uint64_t address = 0;
    uint8_t *ptr = (uint8_t *)&address;
    uint8_t i;
    for (i = 0; i < 6; ++i) {
        ptr[i] = DATAEE_ReadByte(RADIO_PIPE_ADDRESS + i);
    }
    return address;
}

void CONFIG_SetServoPosition(uint8_t index, uint8_t pos)
{
    if (index < SERVO_COUNT) {
        DATAEE_WriteByte(SERVO_POSITION + index, pos);
    }
}

uint8_t CONFIG_GetServoPosition(uint8_t index)
{
    if (index >= SERVO_COUNT) {
        return 0;
    }

    return DATAEE_ReadByte(SERVO_POSITION + index);
}

void CONFIG_SetServoOffset(uint8_t index, uint8_t offset)
{
    if (index < SERVO_COUNT) {
        DATAEE_WriteByte(SERVO_OFFSET + index, offset);
    }
}

uint8_t CONFIG_GetServoOffset(uint8_t index)
{
    if (index >= SERVO_COUNT) {
        return 0;
    }

    return DATAEE_ReadByte(SERVO_OFFSET + index);
}
