# NRF24 RC radio

This project consists of a custom board for controlling RC models using a NRF24L01+ radio.

## Hardware

The hardware consists of a custom board featuring a PIC16F1829 and a NRF24L01+ module.
The board has the following interfaces:

- 4 slots for RC servos or motor controllers
- A serial port for communication
- An ISCP port for programming the PIC16F1829

Kicad files for the custom files are present in the `hardware` directory.

### BOM

| Components      | References | Footprint        | Quantity |
| --------------- | ---------- | ---------------- | -------- |
| NRF24L01+       | U3         |                  | 1        |
| PIC16F1829      | U2         | QFN (4x4x0.9 mm) | 1        |
| MPC1700         | U1         | SOT-23           | 1        |
| Capacitor 100nF | C2         | SMD 0402         | 1        |
| Capacitor 1uF   | C1, C3     | SMD 0402         | 2        |
| Capacitor 10uF  | C4, C5     | SMD 0603         | 2        |
| Resistor 10k    | R1         | SMD 0402         | 1        |

## Firmware

The firmware is written in C and peripheral drivers were generated using MPLAB code configurator. The NRF24 driver was ported from this [repository](https://github.com/nRF24/RF24).

The device can run in 3 modes:

- RC receiver: it receives data from the radio and uses it to control servos
- RX radio: it receives data from the radio and sends it on the serial port
- TX radio: data received from the serial port is sent over the radio

### Instructions for building and flashing firmware

Requirements:

- [xc8 compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)
- make

You can either build the firmware by using MPLAB X IDE or run this command:

```sh
make -C radio-firmware
```

Flash the HEX file using a [PicKit3](https://www.microchip.com/en-us/development-tool/pg164130) or a [PicKit4](https://www.microchip.com/en-us/development-tool/pg164140) by connecting the programmer to the ISCP connector present on the board.

### Configuration

The configuration of the device is stored in the PIC16F1829 EEPROM.
The size of the EEPROM is 256 bytes but only a small is currently in use.

Here is the EEPROM map:

| Address |         Description | Default value | Valid range |
| ------- | ------------------- | ------------- | ----------- |
| 0       |         Magic value |          0xDE |         N/A |
| 1       |       Radio channel |            76 |     0 - 125 |
| 2       |      Radio Datarate |   0 (250KPBS) | 0: 250KPBS, 1: 1MPBS, 2: 2MPBS |
| 3       |  Radio payload size |            32 |        1-32 |
| 4-9     | NRF24 pipe0 address |             1 |      0-2^48 |
| 10      |    Servo 0 position |           128 |     0 - 255 |
| 11      |    Servo 1 position |           128 |     0 - 255 |
| 12      |    Servo 2 position |           128 |     0 - 255 |
| 13      |    Servo 3 position |           128 |     0 - 255 |
| 14      |      Servo 0 offset |            64 |     0 - 128 |
| 15      |      Servo 1 offset |            64 |     0 - 128 |
| 16      |      Servo 2 offset |            64 |     0 - 128 |
| 17      |      Servo 3 offset |            64 |     0 - 128 |
| 18-255  |      Unused         |           N/A |         N/A |

To read and write the configuration of the device, use `scripts/config.py`.

### Serial protocol

The serial port is configured in the following way: 115200 baud, 1 stop bit, no parity bit.

Each command is one byte long followed by data.

| Command              | Value | Description | data length |
| -------------------- | ----- | ----------- | ----------- |
| WRITE_EEPROM         |     0 | Write byte to EEPROM | 2 (address and data) |
| READ_EEPROM          |     1 | Read byte from EEPROM | 1 (address) |
| REQ_FIRMWARE_VERSION |     2 | Request firmware version | 0 |
| FIRMWARE_VERSION     |     3 | Firmware version | 2 (major and minor version) |
| SERVO_ENABLE         |     4 | Send signals on servos | 0 |
| SERVO_DISABLE        |     5 | Stop signals on servos | 0 |
| EEPROM_DATA          |     6 |  Byte read from EEPROM | 1 |
| RADIO_DATA           |     7 | Data read from NRF24 radio | 32 |
| GET_MODE             |     8 | Request device mode | 0 |
| SET_MODE             |     9 | Change device mode | 1 |
| MODE                 |    10 | Current device mode | 1 |
| RESET                |    11 | Reset device | 0 |
