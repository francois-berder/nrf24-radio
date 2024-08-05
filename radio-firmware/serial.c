#include "mcc_generated_files/mcc.h"
#include "serial.h"
#include "version.h"
#include <xc.h>

// State
enum serial_state_t {
    SERIAL_STATE_IDLE,
    SERIAL_STATE_RECEIVE_DATA,
};

#define CMD_BUFFER_SIZE         (4)

struct cmd_queue_t {
    struct serial_cmd_t cmds[CMD_BUFFER_SIZE];
    uint8_t ptr;
    uint8_t length;
};
static struct cmd_queue_t queue;
static struct serial_cmd_t rx_cmd;
static uint8_t data_received_count = 0;
static enum serial_state_t state = SERIAL_STATE_IDLE;

static const uint8_t cmd_data_length[] = {
    2, // WRITE_EEPROM
    1, // READ_EEPROM
    0, // REQ_FIRMWARE_VERSION
    2, // FIRMWARE_VERSION
    0, // START_SERVO
    0, // STOP_SERVO
    1, // EEPROM_DATA
    32, // RADIO_DATA
    0, // GET_MODE
    1, // SET_MODE
    1, // MODE
    0, // RESET
};

static void push_cmd(struct serial_cmd_t *cmd)
{
    if (queue.length == CMD_BUFFER_SIZE) {
        return;
    }

    uint8_t index = queue.ptr + queue.length;
    if (index >= CMD_BUFFER_SIZE) {
        index -= CMD_BUFFER_SIZE;
    }

    uint8_t *src = (uint8_t *)cmd;
    uint8_t *dst = (uint8_t *)&queue.cmds[index];
    for (uint8_t i = 0; i < sizeof(struct serial_cmd_t); ++i) {
        *dst++ = *src++;
    }

    queue.length++;
}

static void pop_cmd(struct serial_cmd_t *cmd)
{
    uint8_t *src = (uint8_t *)&queue.cmds[queue.ptr];
    uint8_t *dst = (uint8_t *)cmd;
    for (uint8_t i = 0; i < sizeof(struct serial_cmd_t); ++i) {
        *dst++ = *src++;
    }

    queue.length--;
    queue.ptr++;
    if (queue.ptr == CMD_BUFFER_SIZE) {
        queue.ptr = 0;
    }
}

static void handle_id(uint8_t id)
{
    switch (id) {
    case WRITE_EEPROM:
    case READ_EEPROM:
    case FIRMWARE_VERSION:
    case RADIO_DATA:
    case EEPROM_DATA:
    case SET_MODE:
    case MODE:
        state = SERIAL_STATE_RECEIVE_DATA;
        rx_cmd.id = id;
        data_received_count = 0;
        break;
    case REQ_FIRMWARE_VERSION:
    case START_SERVO:
    case STOP_SERVO:
    case GET_MODE:
    case RESET:
        rx_cmd.id = id;
        push_cmd(&rx_cmd);
        break;
    default:
        /* Ignore byte */
        break;
    }
}

static void handle_data(uint8_t data)
{
    rx_cmd.raw_data[data_received_count++] = data;

    if (data_received_count >= cmd_data_length[rx_cmd.id]) {
        push_cmd(&rx_cmd);
        state = SERIAL_STATE_IDLE;
        data_received_count = 0;
    }
}

void SERIAL_Process(void)
{
    if (!EUSART_is_rx_ready()) {
        return;
    }

    uint8_t byte = EUSART_Read();

    switch (state) {
    case SERIAL_STATE_IDLE:
        handle_id(byte);
        break;
    case SERIAL_STATE_RECEIVE_DATA:
        handle_data(byte);
        break;
    }
}

int SERIAL_Receive(struct serial_cmd_t *cmd)
{
    if (queue.length == 0) {
        return 0;
    }

    pop_cmd(cmd);

    return 1;
}

void SERIAL_Send(struct serial_cmd_t *cmd)
{
    EUSART_Write(cmd->id);

    uint8_t length = cmd_data_length[cmd->id];
    for (uint8_t i = 0; i < length; ++i) {
        EUSART_Write(cmd->raw_data[i]);
    }
}
