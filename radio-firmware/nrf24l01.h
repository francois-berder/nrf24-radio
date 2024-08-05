/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NRF24L01_H
#define	NRF24L01_H

#include <stdbool.h>
#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

enum NRF24_DataRate {
    NRF24_DATARATE_250KBPS,
    NRF24_DATARATE_1MBPS,
    NRF24_DATARATE_2MBPS,
};

enum NRF24_PALevel {
    NRF24_PA_MIN,
    NRF24_PA_LOW,
    NRF24_PA_HIGH,
    NRF24_PA_MAX,
};

enum NRF24_CRCLength {
    NRF24_CRC_DISABLED,
    NRF24_CRC_8,
    NRF24_CRC_16
};

void NRF24L01_Initialize(uint8_t channel, enum NRF24_DataRate datarate);

void NRF24L01_SetPALevel(enum NRF24_PALevel level);

void NRF24L01_SetPayloadSize(uint8_t size);

void NRF24L01_SetCRCLength(enum NRF24_CRCLength length);

void NRF24L01_StartListening(void);

void NRF24L01_StopListening(void);

void NRF24L01_OpenReadingPipe(uint8_t child, uint64_t address);
void NRF24L01_OpenWritingPipe(uint64_t address);

void NRF24L01_CloseReadingPipe(uint8_t child);

bool NRF24L01_AvailableData(void);

void NRF24L01_Read(void *buffer, size_t length);

void NRF24L01_Write(const void *buffer, size_t length);

void NRF24L01_PrintRegs(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* NRF24L01_H */

