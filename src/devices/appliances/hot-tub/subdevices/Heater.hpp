#ifndef _HOT_TUB_HEATER_HPP_
#define _HOT_TUB_HEATER_HPP_

#include <core/Device.hpp>
#include <types/device_types.h>

class Heater : public Device {

private:
    Heater(const Heater&);
    Heater& operator=(const Heater&);

    uint16_t device_id_;
    DeviceMode mode_;
    bool manual_control_;
    NetAddr controller_net_addr_;

public:
    Heater( string name = "Heater",
            uint16_t device_id = DEVICE_ID_NOT_INIT,
            DeviceMode mode = DeviceModeNotInit,
            bool manual_control = false,
            NetAddr controller_net_addr = 0)
      : Device(name),
        device_id_(device_id),
        mode_(mode),
        manual_control_(manual_control),
        controller_net_addr_(controller_net_addr)
      {};

    Heater(uint16_t device_id, NetAddr controller_net_addr)
      : Device(string("Heater")),
        device_id_(device_id),
        mode_(DeviceModeNotInit),
        manual_control_(false),
        controller_net_addr_(controller_net_addr)
      {};

    uint16_t getDeviceID() const;
    // No setter for device ID
    DeviceMode getDeviceMode() const;
    void setDeviceMode(DeviceMode mode);
    bool isManualControl() const;
    void setManualControl(bool manual_control);
    NetAddr getControllerNetAddr() const;
    void setControllerNetAddr(NetAddr controller_net_addr);
};

#endif /* _HOT_TUB_HEATER_HPP_ */
