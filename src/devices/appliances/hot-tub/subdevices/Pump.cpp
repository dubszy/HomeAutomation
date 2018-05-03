#include <devices/appliances/hot-tub/subdevices/Pump.hpp>

uint16_t Pump::getDeviceID() const {
    return device_id_;
}

DeviceMode Pump::getDeviceMode() const {
    return mode_;
}

void Pump::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

DeviceModeFlag Pump::getDeviceModeFlags() const {
    return mode_flags_;
}

void Pump::setDeviceModeFlags(DeviceModeFlag mode_flags) {
    mode_flags_ = mode_flags;
}

bool Pump::isManualControl() const {
    return manual_control_;
}

void Pump::setManualControl(bool manual_control) {
    manual_control_ = manual_control;
}

NetAddr Pump::getControllerNetAddr() const {
    return controller_net_addr_;
}

void Pump::setControllerNetAddr(NetAddr controller_net_addr) {
    controller_net_addr_ = controller_net_addr;
}
