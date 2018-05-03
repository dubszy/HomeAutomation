#ifndef _HOT_TUB_BLOWER_HPP_
#define _HOT_TUB_BLOWER_HPP_

#include <core/Device.hpp>
#include <types/device_types.h>

class Blower : public Device {

private:
    Blower(const Blower&);
    Blower& operator=(const Blower&);

    uint16_t device_id_;
    DeviceMode mode_;
    bool manual_control_;
    NetAddr controller_net_addr_;

public:
    Blower( string name = "Blower",
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

    Blower(uint16_t device_id, NetAddr controller_net_addr)
      : Device(string("Blower")),
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

#endif /* _HOT_TUB_BLOWER_HPP_ */
