#include "HotTub.hpp"

#include <errno.h>
#include <fcntl.h>

HotTub::HotTub(string name) : Appliance(name) {
    client_ = new SPIDevice(0, 1000000, 16);

    zone0_thermo_ = new DS18B20(string("Heater Thermometer"), 0);
    zone1_thermo_ = new DS18B20(string("Pump 1 Zone Thermometer"), 0);
    zone2_thermo_ = new DS18B20(string("Pump 2 Zone Thermometer"), 0);
    zone3_thermo_ = new DS18B20(string("Footwell Thermometer"), 0);

    heater_ = new Heater(DEVICE_ID_HEATER, NET_ADDR);
    pump1_ = new Pump(string("Pump 1"), DEVICE_ID_PUMP1, NET_ADDR);
    pump2_ = new Pump(string("Pump 2"), DEVICE_ID_PUMP2, NET_ADDR);
    blower_ = new Blower(DEVICE_ID_BLOWER, NET_ADDR);
}

HotTub::~HotTub() {
    delete client_;
    delete zone0_thermo_;
    delete zone1_thermo_;
    delete zone2_thermo_;
    delete zone3_thermo_;
    delete heater_;
    delete pump1_;
    delete pump2_;
    delete blower_;
}

int HotTub::clientWrite(ClientTransaction txn) {
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

    return client_->transact16(data, 9);
}

int HotTub::changeDeviceMode(BasicDeviceInfo device_info) {
    ClientTransaction txn = {};

    txn.txn_type = TransactionTypeControllerChangeDeviceMode;
    txn.device_info = device_info;
    return clientWrite(txn);
}

int HotTub::changeHeaterMode(DeviceMode mode) {
    BasicDeviceInfo device_info;
    device_info.device_id = DEVICE_ID_HEATER;
    device_info.mode = mode;
    return changeDeviceMode(device_info);
}

int HotTub::changePump1Mode(DeviceMode mode, DeviceModeFlag mode_flags) {
    BasicDeviceInfo device_info;
    device_info.device_id = DEVICE_ID_PUMP1;
    device_info.mode = mode;
    device_info.mode_flags = mode_flags;
    return changeDeviceMode(device_info);
}

int HotTub::changePump2Mode(DeviceMode mode) {
    BasicDeviceInfo device_info;
    device_info.device_id = DEVICE_ID_PUMP2;
    device_info.mode = mode;
    return changeDeviceMode(device_info);
}

int HotTub::changeBlowerMode(DeviceMode mode) {
    BasicDeviceInfo device_info;
    device_info.device_id = DEVICE_ID_BLOWER;
    device_info.mode = mode;
    return changeDeviceMode(device_info);
}

