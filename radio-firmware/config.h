/* 
 * File:   config.h
 * Author: francois
 *
 * Created on April 25, 2023, 9:09 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "nrf24l01.h"

#ifdef	__cplusplus
extern "C" {
#endif

void CONFIG_Initialize(void);

void CONFIG_SetRadioChannel(uint8_t channel);
uint8_t CONFIG_GetRadioChannel(void);

void CONFIG_SetRadioDataRate(enum NRF24_DataRate datarate);
enum NRF24_DataRate CONFIG_GetRadioDataRate(void);

void CONFIG_SetPayloadSize(uint8_t payload_size);
uint8_t CONFIG_GetPayloadSize(void);

void CONFIG_SetPipeAddress(uint64_t address);
uint64_t CONFIG_GetPipeAddress(void);

void CONFIG_SetServoPosition(uint8_t index, uint8_t pos);
uint8_t CONFIG_GetServoPosition(uint8_t index);

void CONFIG_SetServoOffset(uint8_t index, uint8_t offset);
uint8_t CONFIG_GetServoOffset(uint8_t index);

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

