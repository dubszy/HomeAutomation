#include <stdint.h>

typedef uint16_t NetAddr;

#define _TXN_START  0xFACE
#define _TXN_END    0xE0F0

typedef enum {
    TransactionTypeNone                       = 0x0000,
    TransactionTypeSysMode                    = 0x0001,
    TransactionTypeControllerChangeDeviceMode = 0x0002,
    TransactionTypeWarning                    = 0x0003,
    TransactionTypeWarningResponse            = 0x0004,
    TransactionTypeAck                        = 0xD000,
    TransactionTypeNoStart                    = 0xEFF0,
    TransactionTypeEmpty                      = 0xEFF1,
    TransactionTypeTooLarge                   = 0xEFF2,
    TransactionTypeInvalidChecksum            = 0xEFF3,
    TransactionTypeInvalidNetAddr             = 0xEFF4,
    TransactionTypeMask                       = 0xFFFF
} TransactionType;

typedef enum {
    SysModeStarting = 0x0001,
    SysModeReady    = 0x0002,
    SysModeStopping = 0x0003,
    SysModeError    = 0xEFF0,
    SysModeMask     = 0xFFFF
} SysMode;

typedef enum {
    WarningInvalidDeviceMode                      = 0x0001,
    WarningInvalidDeviceModeFlags                 = 0x0002,
    WarningOtherControllerChangeDeviceMode        = 0x0004,
    WarningConfirmOtherControllerChangeDeviceMode = 0x0008,
    WarningOtherDeviceMustChangeMode              = 0x0010,
    WarningMask                                   = 0xFFFF
} Warning;

/**
 * Compute a 16-bit one's complement checksum of supplied data.
 *
 * @param data Array of 16-bit data to compute the checksum of
 * @param len Total length of {@code data} or length of {@code data}
 *        to compute the checksum on
 *
 * @return The 16-bit one's complement checksum of {@code data}
 */
inline uint16_t compute_checksum(uint16_t *data, uint16_t len) const {

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
inline bool validate_checksum(uint16_t *data, uint16_t checksum) const {
    if (compute_checksum(data, (sizeof(data) / 2)) == checksum) {
        return true;
    }
    return false;
}

/**
 * Validate that a device is allowed to control this device (by
 * checking a supplied network address against a list of valid
 * network addresses).
 *
 * @param net_addr The network address to check against
 * @param valid_net_addrs An array of valid network addresses
 *
 * @return {@code true} if {@code net_addr} matches any network
 * address contained within {@code valid_net_addrs}, {@code false}
 * otherwise
 */
bool validate_net_addr(uint16_t net_addr, uint16_t *valid_net_addrs) {
    for (int i = 0; i < (sizeof(valid_net_addrs) / 2); i++) {
        if (valid_net_addrs[i] == net_addr) {
            return true;
        }
    }
    return false;
}
