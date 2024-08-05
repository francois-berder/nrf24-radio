#!/usr/bin/env python3

import argparse
import json
import time
from enum import IntEnum

import jsonschema
import serial

EEPROM_SIZE = 256  # In bytes

# Commands
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

# Config default values
CONFIG_MAGIC_VALUE = 222
RADIO_DEFAULT_CHANNEL = 0
RADIO_DEFAULT_PAYLOAD_SIZE = 32
RADIO_DEFAULT_PIPE_ADDRESS = 0
SERVO_DEFAULT_POSITION = 0
SERVO_DEFAULT_OFFSET = 64

config_schema = {
    "type": "object",
    "properties": {
        "rf_channel": {"type": "integer", "minimum": 0, "maximum": 125},
        "rf_datarate": {"type": "integer"},
        "rf_payload": {"type": "integer", "minimum": 4, "maximum": 32},
        "rf_pipe_address": {"type": "integer", "minimum": 0},
        "servo_positions": {"type": "array", "items": {"type": "integer", "minimum": 0, "maximum": 255}, "minItems": 4, "maxItems": 4},
        "servo_offsets": {"type": "array", "items": {"type": "integer", "minimum": 0, "maximum": 128}, "minItems": 4, "maxItems": 4},
    },
    "required": ["rf_channel", "rf_datarate", "rf_payload", "rf_pipe_address", "servo_positions", "servo_offsets"],
}


class RadioDatarate(IntEnum):
    RADIO_DATARATE_250KBPS = 0
    RADIO_DATARATE_1MPBS = 1
    RADIO_DATARATE_2MPBS = 2


class RadioConfiguration:
    def __init__(self):
        self.rf_channel: int = RADIO_DEFAULT_CHANNEL
        self.rf_datarate: RadioDatarate = RadioDatarate.RADIO_DATARATE_250KBPS
        self.rf_payload: int = RADIO_DEFAULT_PAYLOAD_SIZE
        self.rf_pipe_address: int = RADIO_DEFAULT_PIPE_ADDRESS
        self.servo_positions: [int] = [SERVO_DEFAULT_POSITION] * 4
        self.servo_offsets: [int] = [SERVO_DEFAULT_OFFSET] * 4

    def from_bytes(self, data: bytes):
        if data[0] != CONFIG_MAGIC_VALUE:
            print('Invalid magic value!')

        self.rf_channel = data[1]
        self.rf_datarate = data[2]
        self.rf_payload = data[3]
        self.rf_pipe_address = int.from_bytes(data[4:10], byteorder='little')
        self.servo_positions = list(data[10:14])
        self.servo_offsets = list(data[14:18])

    def to_bytes(self):
        data = [255] * EEPROM_SIZE
        data[0] = CONFIG_MAGIC_VALUE
        data[1] = self.rf_channel
        data[2] = self.rf_datarate
        data[3] = self.rf_payload
        data[4] = self.rf_pipe_address & 0xFF
        data[5] = (self.rf_pipe_address >> 8) & 0xFF
        data[6] = (self.rf_pipe_address >> 16) & 0xFF
        data[7] = (self.rf_pipe_address >> 24) & 0xFF
        data[8] = (self.rf_pipe_address >> 32) & 0xFF
        data[9] = (self.rf_pipe_address >> 40) & 0xFF
        data[10] = self.servo_positions[0]
        data[11] = self.servo_positions[1]
        data[12] = self.servo_positions[2]
        data[13] = self.servo_positions[3]
        data[14] = self.servo_offsets[0]
        data[15] = self.servo_offsets[1]
        data[16] = self.servo_offsets[2]
        data[17] = self.servo_offsets[3]

        return bytes(data)

    def export_to_file(self, filename: str):
        with open(filename, 'w', encoding="utf-8") as f:
            json.dump(vars(self), f)

    def import_from_file(self, filename: str):
        with open(filename, 'r', encoding="utf-8") as f:
            data = json.load(f)
            jsonschema.validate(instance=data, schema=config_schema)
            self.__dict__.update(data)

    def __str__(self):
        return '''channel: {}MHz
datarate: {}
payload size: {}
pipe 0 address: {}
servo positions: {}
servo offsets: {}'''.format(2400 + self.rf_channel, self.rf_datarate, self.rf_payload, self.rf_pipe_address, self.servo_positions, self.servo_offsets)


def read_config(args):
    ser = serial.Serial(args.device_name, 115200)
    ser.write(REQ_FIRMWARE_VERSION.to_bytes(1, 'little'))
    out = ser.read(size=3)

    fw_version_major = out[1]
    fw_version_minor = out[2]

    print('FW version: {}.{}'.format(fw_version_major, fw_version_minor))
    print('Read configuration from device...')

    # Read entire EEPROM
    eeprom_content = [0] * EEPROM_SIZE
    for i in range(EEPROM_SIZE):
        ser.write(bytes([READ_EEPROM, i]))
        output = ser.read(size=2)
        if output[0] == EEPROM_DATA:
            eeprom_content[i] = output[1]

    # Decode EEPROM content and save it to file
    config = RadioConfiguration()
    config.from_bytes(bytes(eeprom_content))
    config.export_to_file(args.output_file)


def write_config(args):
    ser = serial.Serial(args.device_name, 115200)
    ser.write(REQ_FIRMWARE_VERSION.to_bytes(1, 'little'))
    out = ser.read(size=3)

    config = RadioConfiguration()
    config.import_from_file(args.input_file)
    data = config.to_bytes()
    print('Writing configuration to device...')

    for i in range(EEPROM_SIZE):
        ser.write(bytes([WRITE_EEPROM, i, data[i]]))
        # It takes a bit of time to write on EEPROM
        time.sleep(0.05)


def main():
    parser = argparse.ArgumentParser(prog='Radio configurator')
    parser.add_argument('device_name')
    subparsers = parser.add_subparsers()
    parser_read = subparsers.add_parser(
        'read', help='Read configuration from device')
    parser_read.add_argument('output_file')
    parser_read.set_defaults(func=read_config)
    parser_write = subparsers.add_parser(
        'write', help='Write configuration to device')
    parser_write.add_argument('input_file')
    parser_write.set_defaults(func=write_config)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
