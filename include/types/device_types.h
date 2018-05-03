#ifndef _DEVICE_TYPES_H_
#define _DEVICE_TYPES_H_

#include <devices/core/Device.hpp>
#include "network_types.h"

#define DEVICE_ID_NOT_INIT  0xEFF0
#define DEVICE_ID_MASK      0xFFFF

typedef enum {
    DeviceModeOff     = 0x0000,
    DeviceModeOn      = 0x0001,
    DeviceModeNotInit = 0xEFF0,
    DeviceModeErr     = 0xEFF1,
    DeviceModeMask    = 0xFFFF
} DeviceMode;

typedef enum {
    DeviceModeFlagNone = 0x0000,
    DeviceModeFlagLow  = 0x0001,
    DeviceModeFlagHigh = 0x0002,
    DeviceModeFlagMask = 0xFFFF
} DeviceModeFlag;

typedef struct {
    uint16_t device_id;
    DeviceModeFlag mode_flags;
    DeviceMode mode;
} BasicDeviceInfo;

typedef struct {
    BasicDeviceInfo info;
    NetAddr controller_net_addr;
    bool manual_control;
} DeviceData;

typedef struct {
    TransactionType txn_type;
    SysMode sys_mode;
    Warning warnings;
    BasicDeviceInfo device_info;
} ClientTransaction;

typedef struct {
    NetAddr net_addr;
    TransactionType txn_type;
    SysMode sys_mode;
    Warning warnings;
    DeviceData device;
} Transaction;
