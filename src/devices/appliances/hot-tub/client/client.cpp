
/*
 * A somewhat dirty fix for when the Arduino editor is not being used:
 * If we're using the Arduino editor, Arduino_h will already be defined,
 * as <Arduino.h> is included by default (as will most other headers
 * that this file needs). If another editor is being used, the headers
 * below will not be included by default, and will result in errors.
 * Note that __has_include() cannot be used because that macro is not
 * defined in the Arduino environment.
 */
#ifdef Arduino_h
#include <SPI.h>
#else
#include <tools/avr/lib/gcc/avr/4.9.2/include/stdarg.h>
#include <tools/avr/lib/gcc/avr/4.9.2/include/stdint.h>
#include <arduino/avr/cores/arduino/Arduino.h>
#include <arduino/avr/libraries/SPI/src/SPI.h>
#endif

#include <types/device_types.h>

/* Pins */
#define HEATER_PIN          3
#define PUMP1_LOW_PIN       5
#define PUMP1_HIGH_PIN      6
#define PUMP2_PIN           9
#define BLOWER_PIN          10

#define NET_ADDR            0x3221

/* Devices */
#define DEVICE_ID_HEATER    0x0001
#define DEVICE_ID_PUMP1     0x0002
#define DEVICE_ID_PUMP2     0x0003
#define DEVICE_ID_BLOWER    0x0004
#define DEVICE_ID_MASK      0xFFFF

uint16_t _heaterMode;   /**< Current mode of the heater */
uint16_t _pump1Mode;    /**< Current mode of pump 1 */
uint16_t _pump1Flags;   /**< Current mode flags of pump 1 */
uint16_t _pump2Mode;    /**< Current mode of pump 2 */
uint16_t _blowerMode;   /**< Current mode of the blower fan */

void pulsePin(uint8_t pin) {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}

void resetPins() {
    digitalWrite(HEATER_PIN, LOW);
    digitalWrite(PUMP1_LOW_PIN, LOW);
    digitalWrite(PUMP1_HIGH_PIN, LOW);
    digitalWrite(PUMP2_PIN, LOW);
    digitalWrite(BLOWER_PIN, LOW);
}

/**
 * Deficient implementation of printf for writing
 * out to the serial port.
 *
 * @param format const char* formatted in printf-esque fashion
 * @param ... parameters to supply to {@code format}
 */
void serial_print(const char *format, ...) {
    bool found_fmt = false;

    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            found_fmt = true;
            format++;
            continue;
        }
        if (found_fmt == true) {
            switch (*format) {
                case 'd':
                    Serial.print(va_arg(args, int));
                    break;
                case 'h':
                    Serial.print("0x");
                    Serial.print(va_arg(args, int), HEX);
                    break;
                case 's':
                    Serial.print(va_arg(args, const char *));
                    break;
                default:
                    break;
            }
            found_fmt = false;
        } else {
            Serial.print(*format);
        }
        format++;
    }
    va_end(args);
}

/**
 * Write a {@link Transaction} to the SPI bus.
 *
 * @param txn {@link Transaction} to write
 */
void spi_write(ClientTransaction txn) {
    uint16_t data[9] = {
            _TXN_START,
            txn.txn_type,
            txn.sys_mode,
            txn.warnings,
            txn.device_info.device_id,
            txn.device_info.mode,
            txn.device_info.mode_flags};

    data[7] = _TXN_END;
    data[8] = compute_checksum(data, 8);

    for (int i = 0; i < 10; i++) {
        SPI.transfer16(data[i]);
    }
}

/**
 * Read the SPI bus and build out a {@link Transaction} if one is found.
 * (Writes 16 bits to the SPI bus, and checks the response.)
 *
 * @return {TransactionTypeNone} if nothing is returned from the bus
 * @return {TransactionTypeNoStart} if the first word read is not {@link _TXN_START}
 * @return {TransactionTypeTooLarge} if 6 words were read after _TXN_START without
 *         encountering {@link _TXN_END}
 * @return {TransactionTypeEmpty} if no words were read in between _TXN_START and _TXN_END.
 * @return {TransactionTypeInvalidChecksum} if the word after _TXN_END does not match
 *         the 16-bit one's complement checksum of the data read for the
 *         current transaction
 * @return {@link Transaction} built from the data read from the SPI bus
 */
ClientTransaction spi_read() {
    uint16_t buf[6] = {};
    uint16_t word;

    ClientTransaction txn;

    word = SPI.transfer16(0);

    if (!word) {
        return {TransactionTypeNone};
    } else if (word == _TXN_START) {
        uint8_t index = 0;
        while ((word = SPI.transfer16(0)) != _TXN_END) {
            buf[index] = word;
            ++index;
            if (index > 5) {
                serial_print("A single transaction cannot be larger than 6 words (12 bytes)\n");
                return {TransactionTypeTooLarge};
            }
        }

        if (index == 0) {
            serial_print("Empty transaction");
            return {TransactionTypeEmpty};
        }

        if (!validate_checksum(buf, SPI.transfer(0))) {
            serial_print("Corrupted transaction, failed to validate checksum\n");
            return {TransactionTypeInvalidChecksum};
        }

        txn.txn_type          = buf[0];
        txn.sys_mode          = buf[1];
        txn.warnings          = buf[2];
        txn.device_info.device_id  = buf[3];
        txn.device_info.mode       = buf[4];
        txn.device_info.mode_flags = buf[5];

        serial_print("Received transaction:\n\tTransaction Type: %h\n\tSystem Mode: %h\n\tWarnings: %h\n\tDevice:\n\t\tID: %h\n\t\tMode: %h\n\t\tMode Flags: %h\n",
                txn.txn_type, txn.sys_mode, txn.warnings, txn.device_info.device_id, txn.device_info.mode, txn.device_info.mode_flags);

        return txn;
    } else {
        serial_print("Corrupted transaction, transfers must begin with _TXN_START (%h), transfer began with: %h\n", _TXN_START, word);
        return {TransactionTypeNoStart};
    }
}

void heater_control(uint16_t mode) {

    ClientTransaction txn = {};

    // If the heater mode cannot be determined, abort
    if (_heaterMode == DeviceModeErr) {
        serial_print("Cannot determine heater mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
        return;
    }

    // If pump 1's mode cannot be determined, abort
    if (_pump1Mode == DeviceModeErr) {
        serial_print("Cannot determine pump 1 mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
        return;
    }

    if ((mode != DeviceModeOff) && (mode != DeviceModeOn)) {
        serial_print("Invalid mode for heater\n");
        txn.txn_type = TransactionTypeWarning;
        txn.warnings = WarningInvalidDeviceMode;
        txn.device_info.device_id = DEVICE_ID_PUMP1;
        txn.device_info.mode = mode;
        spi_write(txn);
        return;
    }

    if (_heaterMode == DeviceModeOff) {
        if (mode == DeviceModeOn) {
            // Not safe to turn the heater on without pump 1 being on
            if (_pump1Mode == DeviceModeOff) {
                serial_print("Pump 1 must be turned on before heater can be turned on\n");
                txn.txn_type = TransactionTypeWarning;
                txn.warnings = WarningOtherDeviceMustChangeMode;
                txn.device_info.device_id = DEVICE_ID_PUMP1;
                txn.device_info.mode = DeviceModeOn;
                spi_write(txn);
                return;
            } else if (_pump1Mode == DeviceModeOn) {
                pulsePin(HEATER_PIN);
                _heaterMode = mode;
            }
        } else if (mode == DeviceModeOff) {
            serial_print("Heater is already off\n");
        }
    } else if (_heaterMode == DeviceModeOn) {
        if (mode == DeviceModeOff) {
            pulsePin(HEATER_PIN);
            _heaterMode = mode;
        } else if (mode == DeviceModeOn) {
            serial_print("Heater is already on\n");
        }
    }
}

void pump1_control(uint16_t mode, uint16_t mode_flags) {

    ClientTransaction txn = {};

    // If pump 1's mode cannot be determined, abort
    if (_pump1Mode == DeviceModeErr) {
        serial_print("Cannot determine pump 1 mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
        return;
    }

    // If the heater mode cannot be determined, abort
    if (_heaterMode == DeviceModeErr) {
        serial_print("Cannot determine heater mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
        return;
    }

    if ((mode != DeviceModeOff) && (mode != DeviceModeOn)) {
        serial_print("Invalid mode for pump 1\n");
        txn.txn_type = TransactionTypeWarning;
        txn.warnings = WarningInvalidDeviceMode;
        txn.device_info.device_id = DEVICE_ID_PUMP1;
        txn.device_info.mode = mode;
        txn.device_info.mode_flags = mode_flags;
        spi_write(txn);
        return;
    }

    if (_pump1Mode == DeviceModeOff) {
        if (mode == DeviceModeOn) {
            if (mode_flags == DeviceModeFlagLow) {
                pulsePin(PUMP1_LOW_PIN);
                _pump1Mode = mode;
                _pump1Flags = mode_flags;
            } else if (mode_flags == DeviceModeFlagHigh) {
                pulsePin(PUMP1_HIGH_PIN);
                _pump1Mode = mode;
                _pump1Flags = mode_flags;
            } else {
                txn.txn_type = TransactionTypeWarning;
                txn.warnings = WarningInvalidDeviceModeFlags;
                txn.device_info.device_id = DEVICE_ID_PUMP1;
                txn.device_info.mode = mode;
                txn.device_info.mode_flags = mode_flags;
                spi_write(txn);
                return;
            }
        } else if (mode == DeviceModeOff) {
            serial_print("Pump 1 is already off\n");
        }
    } else if (_pump1Mode == DeviceModeOn) {
        if (mode == DeviceModeOff) {
            if (_heaterMode == DeviceModeOff) {
                if (_pump1Flags == DeviceModeFlagLow) {
                    pulsePin(PUMP1_LOW_PIN);
                    _pump1Mode = mode;
                    _pump1Flags = DeviceModeFlagNone;
                } else if (_pump1Flags == DeviceModeFlagHigh) {
                    pulsePin(PUMP1_HIGH_PIN);
                    _pump1Mode = mode;
                    _pump1Flags = DeviceModeFlagNone;
                }
            } else if (_heaterMode == DeviceModeOn) {
                serial_print("Heater must be turned off before pump 1 can be turned off\n");
                txn.txn_type = TransactionTypeWarning;
                txn.warnings = WarningOtherDeviceMustChangeMode;
                txn.device_info.device_id = DEVICE_ID_HEATER;
                txn.device_info.mode = DeviceModeOff;
                spi_write(txn);
            }
        }
    }
}

void pump2_control(DeviceMode mode) {

    ClientTransaction txn = {};

    // If pump 2's mode cannot be determined, abort
    if (_pump2Mode == DeviceModeErr) {
        serial_print("Cannot determine pump 2 mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
    }

    if ((mode != DeviceModeOff) && (mode != DeviceModeOn)) {
        serial_print("Invalid mode for pump 2\n");
        txn.txn_type = TransactionTypeWarning;
        txn.warnings = WarningInvalidDeviceMode;
        txn.device_info.device_id = DEVICE_ID_PUMP2;
        txn.device_info.mode = mode;
        spi_write(txn);
        return;
    }

    if (_pump2Mode == DeviceModeOff) {
        if (mode == DeviceModeOn) {
            pulsePin(PUMP2_PIN);
            _pump2Mode = mode;
        } else if (mode == DeviceModeOff) {
            serial_print("Pump 2 is already off\n");
        }
    } else if (_pump2Mode == DeviceModeOn) {
        if (mode == DeviceModeOff) {
            pulsePin(PUMP2_PIN);
            _pump2Mode = mode;
        } else if (mode == DeviceModeOn) {
            serial_print("Pump 2 is already on\n");
        }
    }
}

void blower_control(DeviceMode mode) {

    ClientTransaction txn = {};

    // If the blower's mode cannot be determined, abort
    if (_blowerMode == DeviceModeErr) {
        serial_print("Cannot determine blower mode\n");
        txn.txn_type = TransactionTypeSysMode;
        txn.sys_mode = SysModeError;
        spi_write(txn);
    }

    if ((mode != DeviceModeOff) && (mode != DeviceModeOn)) {
        serial_print("Invalid mode for blower\n");
        txn.txn_type = TransactionTypeWarning;
        txn.warnings = WarningInvalidDeviceMode;
        txn.device_info.device_id = DEVICE_ID_BLOWER;
        txn.device_info.mode = mode;
        spi_write(txn);
        return;
    }

    if (_blowerMode == DeviceModeOff) {
        if (mode == DeviceModeOn) {
            pulsePin(BLOWER_PIN);
            _blowerMode = mode;
        } else if (mode == DeviceModeOff) {
            serial_print("Blower is already off\n");
        }
    } else if (_blowerMode == DeviceModeOn) {
        if (mode == DeviceModeOff) {
            pulsePin(BLOWER_PIN);
            _blowerMode = mode;
        } else if (mode == DeviceModeOn) {
            serial_print("Blower is already on\n");
        }
    }
}

/**
 * Arduino setup function.
 */
void setup() {
    Serial.begin(9600);
    serial_print("System starting\n");

    SPI.begin();

    ClientTransaction txn;
    txn.txn_type = TransactionTypeSysMode;
    txn.sys_mode = SysModeStarting;
    spi_write(txn);

    pinMode(HEATER_PIN, OUTPUT);
    pinMode(PUMP1_LOW_PIN, OUTPUT);
    pinMode(PUMP1_HIGH_PIN, OUTPUT);
    pinMode(PUMP2_PIN, OUTPUT);
    pinMode(BLOWER_PIN, OUTPUT);
    resetPins();

    txn.sys_mode = SysModeReady;
    spi_write(txn);
}

/**
 * Arduino main loop.
 */
void loop() {
    ClientTransaction rx_txn = spi_read();
    ClientTransaction tx_txn = {};

    switch(rx_txn.txn_type) {
        case TransactionTypeNone:
            return;
        case TransactionTypeNoStart:
        case TransactionTypeEmpty:
        case TransactionTypeTooLarge:
        case TransactionTypeInvalidChecksum:
            serial_print("Corrupted or invalid transaction\n");
            return;
        case TransactionTypeSysMode:
            switch(rx_txn.sys_mode) {
                case SysModeStarting:
                case SysModeReady:
                case SysModeError:
                    tx_txn.txn_type = TransactionTypeAck;
                    spi_write(tx_txn);
                    return;
                case SysModeStopping:
                    tx_txn.txn_type = (TransactionTypeSysMode | TransactionTypeAck);
                    tx_txn.sys_mode = SysModeStopping;
                    spi_write(tx_txn);
                    exit(0);
                case (SysModeStopping | SysModeError):
                    tx_txn.txn_type = (TransactionTypeSysMode | TransactionTypeAck);
                    tx_txn.sys_mode = SysModeStopping;
                    spi_write(tx_txn);
                    exit(-1);
                default:
                    break;
            }
            break;
        case TransactionTypeControllerChangeDeviceMode:
            switch (rx_txn.device_info.device_id) {
                case DEVICE_ID_HEATER:
                    heater_control(rx_txn.device_info.mode);
                    break;
                case DEVICE_ID_PUMP1:
                    pump1_control(rx_txn.device_info.mode, rx_txn.device_info.mode_flags);
                    break;
                case DEVICE_ID_PUMP2:
                    pump2_control(rx_txn.device_info.mode);
                    break;
                case DEVICE_ID_BLOWER:
                    blower_control(rx_txn.device_info.mode);
                    break;
                default:
                    serial_print("Device with ID %h not found\n", rx_txn.device_info.device_id);
                    break;
            }
            break;
        default:
            serial_print("Transaction type with ID %h not applicable for this device\n", rx_txn.txn_type);
            break;
    }
}
