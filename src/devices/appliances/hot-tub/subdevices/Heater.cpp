#include <devices/appliances/hot-tub/subdevices/Heater.hpp>

uint16_t Heater::getDeviceID() const {
    return device_id_;
}

DeviceMode Heater::getDeviceMode() const {
    return mode_;
}

void Heater::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

bool Heater::isManualControl() const {
    return manual_control_;
}

void Heater::setManualControl(bool manual_control) {
    manual_control_ = manual_control;
}

NetAddr Heater::getControllerNetAddr() const {
    return controller_net_addr_;
}

void Heater::setControllerNetAddr(NetAddr controller_net_addr) {
    controller_net_addr_ = controller_net_addr;
}
