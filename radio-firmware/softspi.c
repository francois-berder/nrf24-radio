#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "softspi.h"

void SOFTSPI_ExchangeBlock(void *block, size_t len)
{
    uint8_t *data = (uint8_t *)block;

    while (len) {
        uint8_t tx = *data;
        uint8_t rx = 0;
        for (uint8_t i = 0; i < 8; ++i) {
            /* 1. Set MOSI */
            if (tx & 0x80) {
                SOFTSPI_MOSI_IO_SetHigh();
            } else {
                SOFTSPI_MOSI_IO_SetLow();
            }
            /* 2. Set SCK */
            SOFTSPI_SCK_IO_SetHigh();
            
            rx <<= 1;
            rx |= SOFTSPI_MISO_IO_GetValue() & 0x01;

            SOFTSPI_SCK_IO_SetLow();
            tx <<= 1;
        }
        len--;
        *data++ = rx;
    }
    SOFTSPI_MOSI_IO_SetLow();
}
