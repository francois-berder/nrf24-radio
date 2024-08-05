/*
 * File:   nrf24l01.c
 * Author: francois
 *
 * Created on March 19, 2023, 6:24 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "nrf24l01.h"
#include "softspi.h"

#define _BV(x) (1 << (x))

/* Memory Map */
#define NRF_CONFIG  0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define NRF_STATUS  0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

#define PIPE_COUNT  6

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define CONT_WAVE   7
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5      5
#define DPL_P4      4
#define DPL_P3      3
#define DPL_P2      2
#define DPL_P1      1
#define DPL_P0      0
#define EN_DPL      2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define RF24_NOP      0xFF

#define STARTUP_DELAY (150)
#define POWER_UP_DELAY      (5)
#define POWER_DOWN_DELAY              (2)
#define STANDBYI_DELAY                (2)
#define PRX_MODE_DELAY                (100)
#define MAX_PACKET_SIZE     (32)

#define DEFAULT_CHANNEL     (76)
#define DEFAULT_DATARATE    (NRF24_DATARATE_1MBPS)

static void debug_u8(const char *name, uint8_t tmp)
{
    uint8_t a = tmp >> 4;
    uint8_t b = tmp & 0xF;

    char *buf = (char*)name;
    while (*buf) {
        EUSART_Write(*buf++);
    }
    EUSART_Write(':');
    EUSART_Write(' ');
    EUSART_Write('0');
    EUSART_Write('x');
    if (a < 10) {
        EUSART_Write('0' + a);
    } else {
        EUSART_Write('A' + (a - 10));
    }
    if (b < 10) {
        EUSART_Write('0' + b);
    } else {
        EUSART_Write('A' + (b - 10));
    }
    EUSART_Write('\n');
}

static const uint8_t child_pipe_enable[] = {
    ERX_P0, ERX_P1, ERX_P2,
    ERX_P3, ERX_P4, ERX_P5
};
static const uint8_t child_pipe[] = {
    RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2,
    RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};

static uint8_t _is_p0_rx = 0;
static uint8_t addr_width;
static uint8_t payload_size = 32;
static uint8_t pipe0_reading_address[5]; /* Last address set on pipe 0 for reading. */

static void NRF24L01_Activate(uint8_t code)
{
    uint8_t data[2] = {ACTIVATE, code};
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(data, sizeof(data));
    RF_CS_IO_SetHigh();
}

static uint8_t NRF24L01_Read_Register(uint8_t address)
{
    uint8_t data[2] = {R_REGISTER | (address & 0x1F), RF24_NOP};
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(data, sizeof(data));
    RF_CS_IO_SetHigh();
    return data[1];
}

static void NRF24L01_Read_MultiByte_Register(uint8_t address, void *buffer, uint8_t length)
{
    uint8_t cmd = R_REGISTER | (address & 0x1F);
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(&cmd, sizeof(cmd));
    uint8_t i = length;
    uint8_t *dst = (uint8_t *)buffer;
    while (i--) {
        *dst = RF24_NOP;
        SOFTSPI_ExchangeBlock(dst, 1);
        dst++;
    }
    RF_CS_IO_SetHigh();
}

static void NRF24L01_Write_Register(uint8_t address, uint8_t value)
{
    uint8_t data[2] = {W_REGISTER | (address & 0x1F), value};
    
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(data, sizeof(data));
    RF_CS_IO_SetHigh();
}

static void NRF24L01_Write_MultiByte_Register(uint8_t address, void *buffer, uint8_t length)
{
    uint8_t data = W_REGISTER | (address & 0x1F);
    uint8_t *src = (uint8_t *)buffer;

    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(&data, sizeof(data));
    while (length--) {
        uint8_t tmp = *src++;
        SOFTSPI_ExchangeBlock(&tmp, sizeof(tmp));
    }
    RF_CS_IO_SetHigh();
}

static uint8_t NRF24L01_Send_Command(uint8_t cmd)
{
    cmd |= W_REGISTER;
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(&cmd, 1);
    RF_CS_IO_SetHigh();
    return cmd;
}

static void NRF24L01_SetDataRate(enum NRF24_DataRate datarate)
{
    uint8_t setup = NRF24L01_Read_Register(RF_SETUP);

    switch(datarate) {
    case NRF24_DATARATE_250KBPS:
        setup |= (1 << 5);
        setup &= ~(1 << 3);
        break;
    case NRF24_DATARATE_1MBPS:
        setup &= ~((1 << 5) | (1 << 3));
        break;
    case NRF24_DATARATE_2MBPS:
        setup &= ~(1 << 5);
        setup |= (1 << 3);
        break;
    }

    NRF24L01_Write_Register(RF_SETUP, setup);
}

static void NRF24L01_SetChannel(uint8_t channel)
{
    NRF24L01_Write_Register(RF_CH, channel);
}

void NRF24L01_SetPayloadSize(uint8_t size)
{
    if (size < 1 || size > 32) {
        return;
    }

    payload_size = size;

    // write static payload size setting for all pipes
    for (uint8_t i = 0; i < 6; ++i) {
        NRF24L01_Write_Register(RX_PW_P0 + i, size);
    }
}

static void NRF24L01_SetAddressWidth(uint8_t a_width)
{
    a_width = a_width - 2;
    if (a_width) {
        NRF24L01_Write_Register(SETUP_AW, a_width % 4);
        addr_width = (a_width % 4) + 2;
    }
    else {
        NRF24L01_Write_Register(SETUP_AW, 0);
        addr_width = 2;
    }
}

static void NRF24L01_FlushRX(void)
{
    NRF24L01_Send_Command(FLUSH_RX);
}

static void NRF24L01_FlushTX(void)
{
    NRF24L01_Send_Command(FLUSH_TX);
}

static void NRF24L01_PowerUp(void)
{
    uint8_t config_reg = NRF24L01_Read_Register(NRF_CONFIG);
    
    // if not powered up then power up and wait for the radio to initialize
    if (!(config_reg & (1 << PWR_UP))) {
        config_reg |= 1 << PWR_UP;
        NRF24L01_Write_Register(NRF_CONFIG, config_reg);
        __delay_ms(POWER_UP_DELAY);
    }
}

static void NRF24L01_PowerDown(void)
{
    RF_CE_IO_SetLow();
    uint8_t config = NRF24L01_Read_Register(NRF_CONFIG);
    config &= ~(1 << PWR_UP);
    NRF24L01_Write_Register(NRF_CONFIG, config);
}

void NRF24L01_Initialize(uint8_t channel, enum NRF24_DataRate datarate)
{
    RF_CS_IO_SetHigh();
    RF_CE_IO_SetLow();
    __delay_ms(100);
    __delay_ms(5);
    
    //     // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
    // // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
    // // sizes must never be used. See datasheet for a more complete explanation.

    NRF24L01_Write_Register(SETUP_RETR, (5 << ARD) | 15);
    NRF24L01_SetDataRate(datarate);
    NRF24L01_Activate(0x73);
    NRF24L01_Write_Register(DYNPD, 0);     // disable dynamic payloads by default (for all pipes)
    NRF24L01_Write_Register(EN_AA, 0x3F);  // enable auto-ack on all pipes
    NRF24L01_Write_Register(EN_RXADDR, 0x3); // only open RX pipes 0 & 1
    NRF24L01_SetPayloadSize(32);           // set static payload size to 32 (max) bytes by default
    NRF24L01_SetAddressWidth(5);           // set default address length to (max) 5 bytes
    NRF24L01_SetChannel(channel);

    // Clear flags
    NRF24L01_Write_Register(NRF_STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));

    // Flush buffers
    NRF24L01_FlushRX();
    NRF24L01_FlushTX();

    // Clear CONFIG register:
    //      Reflect all IRQ events on IRQ pin
    //      Enable PRX
    //      Power Up
    //      16-bit CRC (CRC required by auto-ack)
    // Do not write CE high so radio will remain in standby I mode
    NRF24L01_Write_Register(NRF_CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX));
    NRF24L01_PowerUp();
}

void NRF24L01_SetPALevel(enum NRF24_PALevel level)
{
    uint8_t setup = NRF24L01_Read_Register(RF_SETUP);
    switch (level) {
    case NRF24_PA_MIN:
        setup &= ~0x06;
        break;
    case NRF24_PA_LOW:
        setup &= ~0x04;
        setup |= 0x02;
        break;
    case NRF24_PA_HIGH:
        setup |= 0x04;
        setup &= ~0x02;
        break;
    case NRF24_PA_MAX:
        setup |= 0x06;
        break;
    }
    NRF24L01_Write_Register(RF_SETUP, setup);
}

void NRF24L01_SetCRCLength(enum NRF24_CRCLength length)
{
    uint8_t config = NRF24L01_Read_Register(NRF_CONFIG);
    switch (length) {
    case NRF24_CRC_DISABLED:
        config &= ~ (1U << EN_CRC);
        break;
    case NRF24_CRC_8:
        config |= (1U << EN_CRC);
        config &= ~(1U << CRCO);
        break;
    case NRF24_CRC_16:
        config |= (1U << EN_CRC);
        config |= (1U << CRCO);
        break;
    }
    NRF24L01_Write_Register(NRF_CONFIG, config);
}

void NRF24L01_StartListening()
{
    NRF24L01_PowerUp();

    uint8_t config_reg = NRF24L01_Read_Register(NRF_CONFIG);
    config_reg |= (1U << PRIM_RX);
    NRF24L01_Write_Register(NRF_CONFIG, config_reg);
    NRF24L01_Write_Register(NRF_STATUS, (1U << RX_DR) | (1U << TX_DS) | (1U << MAX_RT));
    RF_CE_IO_SetHigh();

    // Restore the pipe0 address, if exists
    if (_is_p0_rx) {
        NRF24L01_Write_MultiByte_Register(RX_ADDR_P0, pipe0_reading_address, addr_width);
    }
    else {
        NRF24L01_CloseReadingPipe(0);
    }
}

void NRF24L01_StopListening(void)
{
    RF_CE_IO_SetLow();

    __delay_us(250);

    uint8_t config = NRF24L01_Read_Register(NRF_CONFIG);
    config &= ~(1 << PRIM_RX);
    NRF24L01_Write_Register(NRF_CONFIG, config);

    uint8_t en_rxaddr = NRF24L01_Read_Register(EN_RXADDR);
    en_rxaddr |= (1U << child_pipe_enable[0]);
    NRF24L01_Write_Register(EN_RXADDR, en_rxaddr);
}

void NRF24L01_OpenReadingPipe(uint8_t child, uint64_t address)
{
    // If this is pipe 0, cache the address.  This is needed because
    // openWritingPipe() will overwrite the pipe 0 address, so
    // startListening() will have to restore it.
    if (child == 0) {
        uint8_t *src = (uint8_t *)&address;
        uint8_t *dst = pipe0_reading_address;
        uint8_t i = addr_width;
        while (i--) {
            *dst++ = *src++;
        }
        _is_p0_rx = true;
    }

    if (child <= 5) {
        // For pipes 2-5, only write the LSB
        if (child < 2) {
            NRF24L01_Write_MultiByte_Register(child_pipe[child], &address, addr_width);
        }
        else {
            NRF24L01_Write_MultiByte_Register(child_pipe[child], &address, 1);
        }

        // Note it would be more efficient to set all of the bits for all open
        // pipes at once.  However, I thought it would make the calling code
        // more simple to do it this way.
        uint8_t en_rxaddr = NRF24L01_Read_Register(EN_RXADDR);
        en_rxaddr |= _BV(child_pipe_enable[child]);
        NRF24L01_Write_Register(EN_RXADDR, en_rxaddr);
    }
}

void NRF24L01_OpenWritingPipe(uint64_t address)
{
    NRF24L01_Write_MultiByte_Register(RX_ADDR_P0, &address, addr_width);
    NRF24L01_Write_MultiByte_Register(TX_ADDR, &address, addr_width);
}

void NRF24L01_CloseReadingPipe(uint8_t pipe)
{
    uint8_t en_rxaddr = NRF24L01_Read_Register(EN_RXADDR);
    en_rxaddr &= ~_BV(child_pipe_enable[pipe]);
    NRF24L01_Write_Register(EN_RXADDR, en_rxaddr);
    if (!pipe) {
        // keep track of pipe 0's RX state to avoid null vs 0 in addr cache
        _is_p0_rx = false;
    }
}

bool NRF24L01_AvailableData(void)
{
    uint8_t status = NRF24L01_Send_Command(RF24_NOP);
    uint8_t pipe = (status >> RX_P_NO) & 0x07;
    return (status & (1 << RX_DR)) && pipe <= 5;
}

void NRF24L01_Read(void *buffer, size_t length)
{
    if (length > payload_size) {
        length = payload_size;
    }

    uint8_t data[MAX_PACKET_SIZE + 1];
    data[0] = R_RX_PAYLOAD;
    uint8_t i;
    for (i = 0; i < MAX_PACKET_SIZE; ++i) {
        data[1 + i] = RF24_NOP;
    }
    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(data, payload_size + 1);
    RF_CS_IO_SetHigh();

    uint8_t *ptr = (uint8_t *)buffer;
    for (i = 0; i < length; ++i) {
        ptr[i] = data[1 + i];
    }

    //Clear the only applicable interrupt flags
    NRF24L01_Write_Register(NRF_STATUS, 1 << RX_DR);
}

void NRF24L01_Write(const void *buffer, size_t length)
{
    if (length > payload_size) {
        length = payload_size;
    }

    uint8_t data[MAX_PACKET_SIZE + 1];
    uint8_t i;
    data[0] = W_TX_PAYLOAD;
    for (i = 0; i < length && i < MAX_PACKET_SIZE; ++i) {
        data[1 + i] = ((const uint8_t*)buffer)[i];
    }
    for (; i < MAX_PACKET_SIZE; ++i) {
        data[1 + i] = RF24_NOP;
    }

    RF_CS_IO_SetLow();
    SOFTSPI_ExchangeBlock(data, MAX_PACKET_SIZE + 1);
    RF_CS_IO_SetHigh();

    RF_CE_IO_SetHigh();

    // TODO: Add timeout
    uint8_t status;
    while (1) {
        status = NRF24L01_Send_Command(RF24_NOP);
        if (status & ((1 << TX_DS) | (1 << MAX_RT))) {
            break;
        }
        __delay_ms(1);
    }

    RF_CE_IO_SetLow();

    NRF24L01_Write_Register(NRF_STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
    if (status & (1 << MAX_RT)) {
        NRF24L01_FlushTX(); // Only going to be 1 packet in the FIFO at a time using this method, so just flush
    }
}

void NRF24L01_PrintRegs(void)
{
    uint8_t status = NRF24L01_Read_Register(NRF_STATUS);
    debug_u8("STATUS", status);

    uint8_t rx_addr_p0[5];
    NRF24L01_Read_MultiByte_Register(RX_ADDR_P0, rx_addr_p0, sizeof(rx_addr_p0));
    debug_u8("RX_ADDR_P0[0]", rx_addr_p0[0]);
    debug_u8("RX_ADDR_P0[1]", rx_addr_p0[1]);
    debug_u8("RX_ADDR_P0[2]", rx_addr_p0[2]);
    debug_u8("RX_ADDR_P0[3]", rx_addr_p0[3]);
    debug_u8("RX_ADDR_P0[4]", rx_addr_p0[4]);

    uint8_t rx_addr_p1[5];
    NRF24L01_Read_MultiByte_Register(RX_ADDR_P1, rx_addr_p1, sizeof(rx_addr_p1));
    debug_u8("RX_ADDR_P1[0]", rx_addr_p1[0]);
    debug_u8("RX_ADDR_P1[1]", rx_addr_p1[1]);
    debug_u8("RX_ADDR_P1[2]", rx_addr_p1[2]);
    debug_u8("RX_ADDR_P1[3]", rx_addr_p1[3]);
    debug_u8("RX_ADDR_P1[4]", rx_addr_p1[4]);

    uint8_t rx_addr_p2 = NRF24L01_Read_Register(RX_ADDR_P2);
    debug_u8("RX_ADDR_P2", rx_addr_p2);
    uint8_t rx_addr_p3 = NRF24L01_Read_Register(RX_ADDR_P3);
    debug_u8("RX_ADDR_P3", rx_addr_p3);
    uint8_t rx_addr_p4 = NRF24L01_Read_Register(RX_ADDR_P4);
    debug_u8("RX_ADDR_P4", rx_addr_p4);
    uint8_t rx_addr_p5 = NRF24L01_Read_Register(RX_ADDR_P5);
    debug_u8("RX_ADDR_P5", rx_addr_p5);

    uint8_t tx_addr[5];
    NRF24L01_Read_MultiByte_Register(TX_ADDR, tx_addr, sizeof(tx_addr));
    debug_u8("TX_ADDR[0]", tx_addr[0]);
    debug_u8("TX_ADDR[1]", tx_addr[1]);
    debug_u8("TX_ADDR[2]", tx_addr[2]);
    debug_u8("TX_ADDR[3]", tx_addr[3]);
    debug_u8("TX_ADDR[4]", tx_addr[4]);

    uint8_t rx_pw_p0 = NRF24L01_Read_Register(RX_PW_P0);
    uint8_t rx_pw_p1 = NRF24L01_Read_Register(RX_PW_P1);
    uint8_t rx_pw_p2 = NRF24L01_Read_Register(RX_PW_P2);
    uint8_t rx_pw_p3 = NRF24L01_Read_Register(RX_PW_P3);
    uint8_t rx_pw_p4 = NRF24L01_Read_Register(RX_PW_P4);
    uint8_t rx_pw_p5 = NRF24L01_Read_Register(RX_PW_P5);
    debug_u8("RX_PW_P0", rx_pw_p0);
    debug_u8("RX_PW_P1", rx_pw_p1);
    debug_u8("RX_PW_P2", rx_pw_p2);
    debug_u8("RX_PW_P3", rx_pw_p3);
    debug_u8("RX_PW_P4", rx_pw_p4);
    debug_u8("RX_PW_P5", rx_pw_p5);

    uint8_t en_aa = NRF24L01_Read_Register(EN_AA);
    debug_u8("EN_AA", en_aa);
    uint8_t en_rxaddr = NRF24L01_Read_Register(EN_RXADDR);
    debug_u8("EN_RXADDR", en_rxaddr);
    uint8_t rf_ch = NRF24L01_Read_Register(RF_CH);
    debug_u8("RF_CH", rf_ch);
    uint8_t rf_setup = NRF24L01_Read_Register(RF_SETUP);
    debug_u8("RF_SETUP", rf_setup);
    uint8_t config = NRF24L01_Read_Register(NRF_CONFIG);
    debug_u8("CONFIG", config);
    uint8_t dynpd = NRF24L01_Read_Register(DYNPD);
    debug_u8("DYNPD", rf_setup);
    uint8_t features = NRF24L01_Read_Register(FEATURE);
    debug_u8("FEATURE", rf_setup);
    uint8_t setup_aw = NRF24L01_Read_Register(SETUP_AW);
    debug_u8("SETUP_AW", setup_aw);
    uint8_t setup_retr = NRF24L01_Read_Register(SETUP_RETR);
    debug_u8("SETUP_RETR", setup_retr);
}
