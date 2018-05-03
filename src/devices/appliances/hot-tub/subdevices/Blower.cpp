#include "../../../../../include/devices/appliances/hot-tub/subdevices/Blower.hpp"

uint16_t Blower::getDeviceID() const {
    return device_id_;
}

DeviceMode Blower::getDeviceMode() const {
    return mode_;
}

void Blower::setDeviceMode(DeviceMode mode) {
    mode_ = mode;
}

bool Blower::isManualControl() const {
    return manual_control_;
}

void Blower::setManualControl(bool manual_control) {
    manual_control_ = manual_control;
}

NetAddr Blower::getControllerNetAddr() const {
    return controller_net_addr_;
}

void Blower::setControllerNetAddr(NetAddr controller_net_addr) {
    controller_net_addr_ = controller_net_addr;
}
