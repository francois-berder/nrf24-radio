#!/usr/bin/env python3

import argparse
import time

import serial

WRITE_EEPROM = 0
READ_EEPROM = 1
REQ_FIRMWARE_VERSION = 2
FIRMWARE_VERSION = 3
START_SERVO = 4
STOP_SERVO = 5
EEPROM_DATA = 6
RADIO_DATA = 7
GET_MODE = 8
SET_MODE = 9
MODE = 10
RESET = 11

# Radio modes
RC_RECEIVER = 0
RX_RADIO = 1
TX_RADIO = 2


def main():
    parser = argparse.ArgumentParser(prog='Send data using the radio')
    parser.add_argument('device_name')
    args = parser.parse_args()

    ser = serial.Serial(args.device_name, 115200, timeout=0.1)

    # 1. Set mode
    ser.write(bytes([SET_MODE, TX_RADIO]))

    # 2. Confirm mode
    ser.write(bytes([GET_MODE]))
    resp = ser.read(size=2)
    print(resp)
    if resp[0] != MODE and resp[1] != TX_RADIO:
        raise RuntimeError('Invalid radio mode')

    # 3. Send data over UART to be sent over the radio
    while True:
        for i in range(10):
            data = [RADIO_DATA] + [i] * 32
            ser.write(bytes(data))
            time.sleep(0.1)


if __name__ == "__main__":
    main()
