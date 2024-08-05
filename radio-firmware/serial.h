/* 
 * File:   serial.h
 * Author: francois
 *
 * Created on April 25, 2023, 9:34 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

// Commands
enum serial_cmd_id_t {
    WRITE_EEPROM,
    READ_EEPROM,
    REQ_FIRMWARE_VERSION,
    FIRMWARE_VERSION,
    START_SERVO,
    STOP_SERVO,
    EEPROM_DATA,
    RADIO_DATA,
    GET_MODE,
    SET_MODE,
    MODE,
    RESET,
};

struct __attribute__((packed)) serial_cmd_t {
    enum serial_cmd_id_t id;
    union __attribute__((packed)) {
        struct __attribute__((packed)) {
            uint8_t addr;
            uint8_t data;
        } write_eeprom_data;
        struct {
            uint8_t addr;
        } read_eeprom_data;
        struct {
            uint8_t data;
        } eeprom_data;
        struct __attribute__((packed)) {
            uint8_t major;
            uint8_t minor;
        } firmware_version;
        struct {
            uint8_t data[32];
        } radio_data;
        struct {
            uint8_t mode;
        } set_mode;
        struct {
            uint8_t mode;
        } mode;
        uint8_t raw_data[32];
    };
};

void SERIAL_Process(void);
int SERIAL_Receive(struct serial_cmd_t *cmd);
void SERIAL_Send(struct serial_cmd_t *cmd);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

