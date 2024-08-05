/*
 * File:   servo.c
 * Author: francois
 *
 * Created on March 24, 2023, 9:51 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "servo.h"

#define SERVO_DEFAULT_VALUE (128)
#define SERVO_DEFAULT_OFFSET (64)
#define TMR2_1MS_PERIOD (124)
#define OFFSET_ZERO (64)

static volatile uint8_t values[SERVO_COUNT] = {
    SERVO_DEFAULT_VALUE,
    SERVO_DEFAULT_VALUE,
    SERVO_DEFAULT_VALUE,
    SERVO_DEFAULT_VALUE
};
static volatile uint8_t offsets[SERVO_COUNT] = {
    SERVO_DEFAULT_OFFSET,
    SERVO_DEFAULT_OFFSET,
    SERVO_DEFAULT_OFFSET,
    SERVO_DEFAULT_OFFSET,
};

static volatile uint8_t current_index = 0;

static void SERVO_Update(void);
static void SERVO_StopSignal(void);

void SERVO_Initialize(void)
{
    TMR1_SetInterruptHandler(SERVO_Update);
    TMR2_SetInterruptHandler(SERVO_StopSignal);
}

void SERVO_Enable(void)
{
    if (T1CONbits.TMR1ON) {
        return;
    }

    /* Wait for TMR2 to be stopped before starting TMR1 */
    while (T2CONbits.TMR2ON);

    current_index = 0;
    TMR1_Reload();
    TMR1_StartTimer();
}

void SERVO_Disable(void)
{
    TMR1_StopTimer();
    /* Note: do not stop TMR2 */
}

void SERVO_SetValue(uint8_t index, uint8_t value)
{
    if (index >= SERVO_COUNT)
        return;

    INTERRUPT_GlobalInterruptDisable();
    values[index] = value;
    INTERRUPT_GlobalInterruptEnable();
}

void SERVO_SetValues(uint8_t *new_values)
{
    INTERRUPT_GlobalInterruptDisable();
    values[0] = new_values[0];
    values[1] = new_values[1];
    values[2] = new_values[2];
    values[3] = new_values[3];
    INTERRUPT_GlobalInterruptEnable();
}

void SERVO_SetOffset(uint8_t index, uint8_t offset)
{
    if (index >= SERVO_COUNT)
        return;

    INTERRUPT_GlobalInterruptDisable();
    offsets[index] = offset;
    INTERRUPT_GlobalInterruptEnable();
}

void SERVO_SetOffsets(uint8_t *new_offsets)
{
    INTERRUPT_GlobalInterruptDisable();
    offsets[0] = new_offsets[0];
    offsets[1] = new_offsets[1];
    offsets[2] = new_offsets[2];
    offsets[3] = new_offsets[3];
    INTERRUPT_GlobalInterruptEnable();
}

// Called every 5ms
static void SERVO_Update(void)
{
    // Setup timer to raise interrupt after 1-2ms
    uint16_t val = TMR2_1MS_PERIOD;

    /* offset in range [-64, 64] */
    val += (offsets[current_index] / 2);
    val -= OFFSET_ZERO;

    val += (values[current_index] / 2);
    if (val >= UINT8_MAX) {
        val = UINT8_MAX;
    }

    TMR2_WriteTimer(0);
    TMR2_LoadPeriodRegister((uint8_t)val);
    TMR2_StartTimer();

    if (current_index == 0) {
        SERVO_1_SIGNAL_SetHigh();
    } else if (current_index == 1) {
        SERVO_2_SIGNAL_SetHigh();
    } else if (current_index == 2) {
        SERVO_3_SIGNAL_SetHigh();
    } else if (current_index == 3) {
        SERVO_4_SIGNAL_SetHigh();
    }
}

static void SERVO_StopSignal(void)
{
    SERVO_1_SIGNAL_SetLow();
    SERVO_2_SIGNAL_SetLow();
    SERVO_3_SIGNAL_SetLow();
    SERVO_4_SIGNAL_SetLow();
    TMR2_StopTimer();

    current_index++;
    if (current_index >= SERVO_COUNT) {
        current_index = 0;
    }
}
