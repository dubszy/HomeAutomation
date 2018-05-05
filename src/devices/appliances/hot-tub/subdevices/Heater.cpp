#include <iomanip>
#include <sstream>

#include <devices/appliances/hot-tub/subdevices/Heater.hpp>

uint16_t Heater::getDeviceID() const {
    return deviceID_;
}

DeviceMode Heater::getDeviceMode() const {
    return mode_;
}

void Heater::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

bool Heater::isManualControl() const {
    return manualControl_;
}

void Heater::setManualControl(bool manualControl) {
    manualControl_ = manualControl;
}

NetAddr Heater::getControllerNetAddr() const {
    return controllerNetAddr_;
}

void Heater::setControllerNetAddr(NetAddr controllerNetAddr) {
    controllerNetAddr_ = controllerNetAddr;
}

string Heater::toString() {
    stringstream sstream;
    sstream << "Heater: { Name: " << name_
            << ", Device ID: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << deviceID_
            << ", Mode: 0x"  << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << mode_
            << ", Manual Control: " << manualControl_
            << ", Controller Net Address: 0x"  << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << controllerNetAddr_
            << " }";
    return sstream.str();
}
