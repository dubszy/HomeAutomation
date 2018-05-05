#include <iomanip>
#include <sstream>

#include <devices/appliances/hot-tub/subdevices/Pump.hpp>

uint16_t Pump::getDeviceID() const {
    return deviceID_;
}

DeviceMode Pump::getDeviceMode() const {
    return mode_;
}

void Pump::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

DeviceModeFlag Pump::getDeviceModeFlags() const {
    return modeFlags_;
}

void Pump::setDeviceModeFlags(DeviceModeFlag modeFlags) {
    modeFlags_ = modeFlags;
}

bool Pump::isManualControl() const {
    return manualControl_;
}

void Pump::setManualControl(bool manualControl) {
    manualControl_ = manualControl;
}

NetAddr Pump::getControllerNetAddr() const {
    return controllerNetAddr_;
}

void Pump::setControllerNetAddr(NetAddr controllerNetAddr) {
    controllerNetAddr_ = controllerNetAddr;
}

string Pump::toString() {
    stringstream sstream;
    sstream << "Pump: { Name: " << name_
            << ", Device ID: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << deviceID_
            << ", Device Mode: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << mode_
            << ", Mode Flags: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std:: hex << modeFlags_
            << ", Manual Control: " << manualControl_
            << ", Controller Net Address: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << controllerNetAddr_
            << " }";
    return sstream.str();
}