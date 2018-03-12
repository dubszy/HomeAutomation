
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

/* Pins */
#define HEATER_PIN      3
#define PUMP1_LOW_PIN   5
#define PUMP1_HIGH_PIN  6
#define PUMP2_PIN       9
#define BLOWER_PIN      10

/* Start and End Transaction */
#define _TXN_START                                      0xFACE
#define _TXN_END                                        0xE0F0

/* Network Address */
#define NET_ADDR                                        0x0000
#define NET_ADDR_MASK                                   0xFFFF

/* Transaction Types */
#define TXN_NONE                                        0x0000
#define TXN_SYS_MODE                                    0x0001
#define TXN_SRVR_CHANGE_DEVICE_MODE                     0x0002
#define TXN_USER_CHANGE_DEVICE_MODE                     0x0003
#define TXN_WARNING                                     0x0004
#define TXN_WARNING_RESPONSE                            0x0005
#define TXN_ACK                                         0xD000
#define TXN_NO_START                                    0xEFF0
#define TXN_EMPTY                                       0xEFF1
#define TXN_TOO_LARGE                                   0xEFF2
#define TXN_INVALID_CHECKSUM                            0xEFF3
#define TXN_INVALID_NET_ADDR                            0xEFF4
#define TXN_TYPE_MASK                                   0xFFFF

/* System Modes - server and client */
#define SYS_MODE_STARTING                               0x0001
#define SYS_MODE_READY                                  0x0002
#define SYS_MODE_STOPPING                               0x0003
#define SYS_MODE_ERROR                                  0x0004
#define SYS_MODE_MASK                                   0xFFFF

/* Warning Flags */
#define WARN_SRVR_OTHER_DEVICE_CHANGE_MODE              0x0001
#define WARN_SRVR_CONFIRM_OTHER_DEVICE_CHANGE_MODE      0x0002
#define WARN_USER_OTHER_DEVICE_CHANGE_MODE              0x0003
#define WARN_USER_CONFIRM_OTHER_DEVICE_CHANGE_MODE      0x0004
#define WARN_USER_SRVR_CHANGE_DEVICE_MODE               0x0005
#define WARN_USER_CONFIRM_SRVR_CHANGE_DEVICE_MODE       0x0006
#define WARN_USER_SRVR_OTHER_DEVICE_CHANGE_MODE         0x0007
#define WARN_USER_CONFIRM_SRVR_OTHER_DEVICE_CHANGE_MODE 0x0008
#define WARN_MASK                                       0xFFFF

/* Devices */
#define DEVICE_HEATER                                   0x0001
#define DEVICE_PUMP1                                    0x0002
#define DEVICE_PUMP2                                    0x0003
#define DEVICE_BLOWER                                   0x0004
#define DEVICE_MASK                                     0xFFFF

/* Device Mode Flags */
#define MODE_FLAG_LOW                                   0x0001
#define MODE_FLAG_HIGH                                  0x0002
#define MODE_FLAG_AUTO                                  0x0004
#define MODE_FLAG_USER                                  0x0008
#define MODE_FLAG_MASK                                  0xFFFF

/* Device Modes */
#define MODE_OFF                                        0x0001
#define MODE_ON                                         0x0002
#define MODE_ERR                                        0x0004
#define MODE_MASK                                       0xFFFF

typedef struct {
    uint16_t device;
    uint16_t mode_flags;
    uint16_t mode;
} DeviceData;

typedef struct {
    uint16_t txn_type;
    uint16_t sys_mode;
    uint16_t warn_flags;
    uint16_t devices_len;
    DeviceData *devices;
} Transaction;

uint16_t valid_net_addrs[1] = {0x0000};

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
 * Compute a 16-bit one's complement checksum of supplied data.
 *
 * @param data Array of 16-bit data to compute the checksum of
 * @param len Total length of {@code data} or length of {@code data}
 *        to compute the checksum on
 *
 * @return The 16-bit one's complement checksum of {@code data}
 */
uint16_t compute_checksum(uint16_t *data, uint16_t len) {

    uint32_t sum;
    int index;

    while (len > 0) {
        sum += data[index];
        index++;
    }

    sum = (sum >> 16) + sum;
    return (uint16_t)(~sum);
}

/**
 * Validate a 16-bit one's complement checksum of supplied data.
 *
 * @param data Array of 16-bit data to validate the checksum of
 * @param checksum The checksum to match against
 *
 * @return {@code true} if {@code checksum} matches the 16-bit
 * one's complement checksum of {@code data}, {@code false} otherwise
 */
bool validate_checksum(uint16_t *data, uint16_t checksum) {
    if (compute_checksum(data, (sizeof(data) / 2)) == checksum) {
        return true;
    }
    return false;
}

/**
 * Validate that a device is allowed to control this device (by
 * checking a supplied network address against a list of valid
 * network addresses contained within this device).
 *
 * @param net_addr The network address to check against
 *
 * @return {@code true} if {@code net_addr} matches any network
 * address contained within {@link valid_net_addrs}, {@code false}
 * otherwise
 */
bool validate_net_addr(uint16_t net_addr) {
    for (int i = 0; i < (sizeof(valid_net_addrs) / 2); i++) {
        if (valid_net_addrs[i] == net_addr) {
            return true;
        }
    }
    return false;
}

uint16_t _heaterMode;   /**< Current mode of the heater */
uint16_t _heaterFlags;  /**< Current mode flags of the heater */
uint16_t _pump1Mode;    /**< Current mode of pump 1 */
uint16_t _pump1Flags;   /**< Current mode flags of pump 1 */
uint16_t _pump2Mode;    /**< Current mode of pump 2 */
uint16_t _pump2Flags;   /**< Current mode flags of pump 2 */
uint16_t _blowerMode;   /**< Current mode of the blower fan */
uint16_t _blowerFlags;  /**< Current mode flags of the blower fan */

/**
 * Write a {@link Transaction} to the SPI bus.
 *
 * @param txn {@link Transaction} to write
 */
void spi_write(Transaction txn) {
    uint16_t data[8 + (txn.devices_len * 3)] = {_TXN_START, NET_ADDR, txn.txn_type, txn.sys_mode, txn.warn_flags, txn.devices_len };
    for (int i = 0; i < txn.devices_len; i++) {
        data[i * 3 + 6] = txn.devices->device;
        data[i * 3 + 7] = txn.devices->mode_flags;
        data[i * 3 + 8] = txn.devices->mode;
    }
    data[(sizeof(data) / 2)] = _TXN_END;
    data[(sizeof(data) / 2)] = compute_checksum(data, (sizeof(data) / 2));

    for (int i = 0; i < (sizeof(data) / 2); i++) {
        SPI.transfer16(data[i]);
    }
}

/**
 * Read the SPI bus and build out a {@link Transaction} if one is found.
 * (Writes 16 bits to the SPI bus, and checks the response.)
 *
 * @return {TXN_NONE} if nothing is returned from the bus
 * @return {TXN_NO_START} if the first word read is not {@link _TXN_START}
 * @return {TXN_INVALID_NET_ADDR} if the second word returned is not a
 *         valid network address
 * @return {TXN_TOO_LARGE} if 128 words were read after _TXN_START and the
 *         network address without encountering {@link _TXN_END}
 * @return {TXN_EMPTY} if no words were read in between _TXN_START, the
 *         network address, and _TXN_END.
 * @return {TXN_INVALID_CHECKSUM} if the word after _TXN_END does not match
 *         the 16-bit one's complement checksum of the data read for the
 *         current transaction
 * @return {@link Transaction} built from the data read from the SPI bus
 */
Transaction spi_read() {
    uint16_t buf[131] = {};
    uint16_t word;
    uint16_t net_addr;

    Transaction txn;

    word = SPI.transfer16(0);

    if (!word) {
        return {0};
    } else if (word == _TXN_START) {
        buf[0] = word;
        word = SPI.transfer16(0);
        if (!validate_net_addr(word)) {
            serial_print("Device with network address %h is not permitted to control this client\n", word);
            return {TXN_INVALID_NET_ADDR};
        }
        buf[1] = net_addr;

        uint8_t index = 0;
        while ((word = SPI.transfer16(0)) != _TXN_END) {
            buf[index + 2] = word;
            ++index;
            if (index > 127) {
                serial_print("A single transaction cannot be larger than 128 words (256 bytes)\n");
                return {TXN_TOO_LARGE};
            }
        }

        if (index == 0) {
            serial_print("Empty transaction");
            return {TXN_EMPTY};
        }

        if (!validate_checksum(buf, SPI.transfer(0))) {
            serial_print("Corrupted transaction, failed to validate checksum\n");
            return {TXN_INVALID_CHECKSUM};
        }

        txn.txn_type = buf[2];
        txn.sys_mode = buf[3];
        txn.warn_flags = buf[4];
        txn.devices_len = buf[5];

        serial_print("Received transaction:\n\tTransaction Type: %h\n\tSystem Mode: %h\n\tWarning Flags: %h\n\tDevices Length: %h\n",
                txn.txn_type, txn.sys_mode, txn.warn_flags, txn.devices_len);

        for (int i = 0; i < txn.devices_len; i++) {
            txn.devices[i].device = buf[i * 3 + 6];
            txn.devices[i].mode_flags = buf[i * 3 + 7];
            txn.devices[i].mode = buf[i * 3 + 8];
            serial_print("\tDevice %d:\n\t\tID: %h\n\t\tMode Flags: %h\n\t\tMode: %h\n",
                    i, txn.devices[i].device, txn.devices[i].mode_flags, txn.devices[i].mode);
        }

        return txn;
    } else {
        serial_print("Corrupted transaction, transfers must begin with _TXN_START (%h), transfer began with: %h\n", _TXN_START, word);
        return {TXN_NO_START};
    }
}

/**
 * Stub function for controlling the heater.
 *
 * @param txn_type {@link Transaction#txn_type} from a heater-related transaction
 * @param warn_flags {@link Transaction#warn_flags} from a heater-related transaction
 * @param devices_len {@link Transaction#devices_len} from a heater-related transaction
 * @param devices {@link Transaction#devices} from a heater-related transaction
 */
void heaterControl(uint16_t txn_type, uint16_t warn_flags, uint16_t devices_len, DeviceData *devices) {
    return; // stub for now
}

/**
 * Stub function for controlling pump 1.
 *
 * @param txn_type {@link Transaction#txn_type} from a pump 1-related transaction
 * @param warn_flags {@link Transaction#warn_flags} from a pump 1-related transaction
 * @param devices_len {@link Transaction#devices_len} from a pump 1-related transaction
 * @param devices {@link Transaction#devices} from a pump 1-related transaction
 */
void pump1Control(uint16_t txn_type, uint16_t warn_flags, uint16_t devices_len, DeviceData *devices) {
    return; // stub for now
}

/**
 * Stub function for controlling pump 2.
 *
 * @param txn_type {@link Transaction#txn_type} from a pump 2-related transaction
 * @param warn_flags {@link Transaction#warn_flags} from a pump 2-related transaction
 * @param devices_len {@link Transaction#devices_len} from a pump 2-related transaction
 * @param devices {@link Transaction#devices} from a pump 2-related transaction
 */
void pump2Control(uint16_t txn_type, uint16_t warn_flags, uint16_t devices_len, DeviceData *devices) {
    return; // stub for now
}

/**
 * Stub function for controlling the blower.
 *
 * @param txn_type {@link Transaction#txn_type} from a blower-related transaction
 * @param warn_flags {@link Transaction#warn_flags} from a blower-related transaction
 * @param devices_len {@link Transaction#devices_len} from a blower-related transaction
 * @param devices {@link Transaction#devices} from a blower-related transaction
 */
void blowerControl(uint16_t txn_type, uint16_t warn_flags, uint16_t devices_len, DeviceData *devices) {
    return; // stub for now
}

/**
 * Arduino setup function.
 */
void setup() {
    Serial.begin(9600);
    serial_print("System starting\n");

    SPI.begin();

    Transaction txn;
    txn.txn_type = TXN_SYS_MODE;
    txn.sys_mode = SYS_MODE_STARTING;
    spi_write(txn);

    pinMode(HEATER_PIN, OUTPUT);
    pinMode(PUMP1_LOW_PIN, OUTPUT);
    pinMode(PUMP1_HIGH_PIN, OUTPUT);
    pinMode(PUMP2_PIN, OUTPUT);
    pinMode(BLOWER_PIN, OUTPUT);
    resetPins();

    txn.sys_mode = SYS_MODE_READY;
    spi_write(txn);
}

/**
 * Arduino main loop.
 */
void loop() {
    Transaction rx_txn = spi_read();
    Transaction tx_txn = {};

    switch(rx_txn.txn_type) {
        case TXN_NONE:
            return;
        case TXN_NO_START:
        case TXN_EMPTY:
        case TXN_TOO_LARGE:
        case TXN_INVALID_CHECKSUM:
        case TXN_INVALID_NET_ADDR:
            serial_print("Corrupted or invalid transaction\n");
            return;
        case TXN_SYS_MODE:
            switch(rx_txn.sys_mode) {
                case SYS_MODE_STARTING:
                case SYS_MODE_READY:
                case SYS_MODE_ERROR:
                    tx_txn.txn_type = TXN_ACK;
                    spi_write(tx_txn);
                    return;
                case SYS_MODE_STOPPING:
                    tx_txn.txn_type = (TXN_SYS_MODE | TXN_ACK);
                    tx_txn.sys_mode = SYS_MODE_STOPPING;
                    spi_write(tx_txn);
                    exit(0);
                case (SYS_MODE_STOPPING | SYS_MODE_ERROR):
                    tx_txn.txn_type = (TXN_SYS_MODE | TXN_ACK);
                    tx_txn.sys_mode = SYS_MODE_STOPPING;
                    spi_write(tx_txn);
                    exit(-2);
                default:
                    break;
            }
            break;
        case TXN_SRVR_CHANGE_DEVICE_MODE:
        case TXN_USER_CHANGE_DEVICE_MODE:
            for (int i = 0; i < rx_txn.devices_len; i++) {
                switch(rx_txn.devices[i].device) {
                    case DEVICE_HEATER:
                        heaterControl(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                        break;
                    case DEVICE_PUMP1:
                        pump1Control(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                        break;
                    case DEVICE_PUMP2:
                        pump2Control(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                        break;
                    case DEVICE_BLOWER:
                        blowerControl(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                        break;
                    default:
                        serial_print("Device with ID %h not found\n", rx_txn.devices[i]);
                        break;
                }
            }
            break;
        case TXN_WARNING_RESPONSE:
            switch(rx_txn.devices[0].device) {
                case DEVICE_HEATER:
                    heaterControl(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                    break;
                case DEVICE_PUMP1:
                    pump1Control(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                    break;
                case DEVICE_PUMP2:
                    pump2Control(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                    break;
                case DEVICE_BLOWER:
                    blowerControl(rx_txn.txn_type, rx_txn.warn_flags, rx_txn.devices_len, rx_txn.devices);
                    break;
                default:
                    serial_print("Device with ID %h not found\n", rx_txn.devices[0]);
                    break;
            }
            break;
        default:
            serial_print("Transaction type with ID %h not found\n", rx_txn.txn_type);
            break;
    }
}
