#include <iomanip>
#include <sstream>

#include <devices/appliances/hot-tub/subdevices/Blower.hpp>

uint16_t Blower::getDeviceID() const {
    return deviceID_;
}

DeviceMode Blower::getDeviceMode() const {
    return mode_;
}

void Blower::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

bool Blower::isManualControl() const {
    return manualControl_;
}

void Blower::setManualControl(bool manualControl) {
    manualControl_ = manualControl;
}

NetAddr Blower::getControllerNetAddr() const {
    return controllerNetAddr_;
}

void Blower::setControllerNetAddr(NetAddr controllerNetAddr) {
    controllerNetAddr_ = controllerNetAddr;
}

string Blower::toString() {
    stringstream sstream;
    sstream << "Pump: { Name: " << name_
            << ", Device ID: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << deviceID_
            << ", Device Mode: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << mode_
            << ", Manual Control: " << manualControl_
            << ", Controller Net Address: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << controllerNetAddr_
            << " }";
    return sstream.str();
}
