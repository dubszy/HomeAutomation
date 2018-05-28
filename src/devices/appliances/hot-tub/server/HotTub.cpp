#include <devices/appliances/hot-tub/server/HotTub.hpp>

#include <iomanip>
#include <sys/stat.h>

#include <Property.hpp>
#include <Properties.hpp>

HotTub::HotTub(string name, NetAddr netAddr)
        : netAddr_(netAddr), Appliance(name) { // NOLINT do not std::move 'name'
    log->debug("Initializing new Hot Tub");
    log->info("Hot Tub: " + toString());
    client_ = new SPIDevice("Client", 0, 1000000, 16);
    log->info("Client: " + client_->toString());

    // Load the .properties file
    string propFilePath = "/etc/homeautomation/devices/appliances/hot-tub/custom.properties";
    struct stat propBuffer;
    if (stat(propFilePath.c_str(), &propBuffer) != 0) {
        propFilePath = "/etc/homeautomation/devices/appliances/hot-tub/default.properties";
    }

    auto *properties = new Properties(propFilePath.c_str());

    zone0_thermo_ = new DS18B20(
            properties->get("thermo.zone0.name")->value(),
            Thermometer::getScaleFromString(properties->get("thermo.zone0.scale")->value()),
            properties->get("thermo.zone0.precision")->valueAsNumber(),
            properties->get("thermo.zone0.addr")->value());
    log->info("Zone 0 Thermo: " + zone0_thermo_->toString());

    zone1_thermo_ = new DS18B20(
            properties->get("thermo.zone1.name")->value(),
            Thermometer::getScaleFromString(properties->get("thermo.zone1.scale")->value()),
            properties->get("thermo.zone1.precision")->valueAsNumber(),
            properties->get("thermo.zone1.addr")->value());
    log->info("Zone 1 Thermo: " + zone1_thermo_->toString());

    zone2_thermo_ = new DS18B20(
            properties->get("thermo.zone2.name")->value(),
            Thermometer::getScaleFromString(properties->get("thermo.zone2.scale")->value()),
            properties->get("thermo.zone2.precision")->valueAsNumber(),
            properties->get("thermo.zone2.addr")->value());
    log->info("Zone 2 Thermo: " + zone2_thermo_->toString());

    zone3_thermo_ = new DS18B20(
            properties->get("thermo.zone3.name")->value(),
            Thermometer::getScaleFromString(properties->get("thermo.zone3.scale")->value()),
            properties->get("thermo.zone3.precision")->valueAsNumber(),
            properties->get("thermo.zone3.addr")->value());
    log->info("Zone 3 Thermo: " + zone3_thermo_->toString());

    heater_ = new Heater(DEVICE_ID_HEATER, netAddr_);
    log->info("Heater: " + heater_->toString());
    pump1_ = new Pump(string("Pump 1"), DEVICE_ID_PUMP1, netAddr_);
    log->info("Pump 1: " + pump1_->toString());
    pump2_ = new Pump(string("Pump 2"), DEVICE_ID_PUMP2, netAddr_);
    log->info("Pump 2: " + pump2_->toString());
    blower_ = new Blower(DEVICE_ID_BLOWER, netAddr_);
    log->info("Blower: " + blower_->toString());
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
            (uint16_t)txn.txn_type,
            (uint16_t)txn.sys_mode,
            (uint16_t)txn.warnings,
            txn.device_info.device_id,
            (uint16_t)txn.device_info.mode,
            (uint16_t)txn.device_info.mode_flags};

    data[7] = _TXN_END;
    data[8] = compute_checksum(data, 8);

    return client_->transact16(data, 9);
}

DS18B20 *HotTub::getZone0Thermo() {
    return zone0_thermo_;
}

DS18B20 *HotTub::getZone1Thermo() {
    return zone1_thermo_;
}

DS18B20 *HotTub::getZone2Thermo() {
    return zone2_thermo_;
}

DS18B20 *HotTub::getZone3Thermo() {
    return zone3_thermo_;
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

void HotTub::selfTest() {
    log->info("Zone 0 Temperature: %f", zone0_thermo_->getTemperature());
    log->info("Zone 1 Temperature: %f", zone1_thermo_->getTemperature());
    log->info("Zone 2 Temperature: %f", zone2_thermo_->getTemperature());
    log->info("Zone 3 Temperature: %f", zone3_thermo_->getTemperature());
}

string HotTub::toString() {
    stringstream sstream;
    sstream << "Hot Tub: { name: " << name_
            << ", Net Address: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << netAddr_
            << " }";
    return sstream.str();
}