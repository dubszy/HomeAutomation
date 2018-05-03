#ifndef _HOT_TUB_PUMP_HPP_
#define _HOT_TUB_PUMP_HPP_

#include <types/device_types.h>

class Pump : public Device {

private:
    Pump(const Pump&);
    Pump& operator=(const Pump&);

    uint16_t device_id_;
    DeviceMode mode_;
    DeviceModeFlag mode_flags_;
    bool manual_control_;
    NetAddr controller_net_addr_;

public:
    Pump( string name = "Pump",
          uint16_t device_id = DEVICE_ID_NOT_INIT,
          DeviceMode mode = DeviceModeNotInit,
          DeviceModeFlag mode_flags = DeviceModeFlagNone,
          bool manual_control = false,
          NetAddr controller_net_addr = 0)
      : Device(name),
        device_id_(device_id),
        mode_(mode),
        mode_flags_(mode_flags),
        manual_control_(manual_control),
        controller_net_addr_(controller_net_addr)
      {};

    Pump(string name, uint16_t device_id, NetAddr controller_net_addr)
      : Device(string(name)),
        device_id_(device_id),
        mode_(DeviceModeNotInit),
        mode_flags_(DeviceModeFlagNone),
        manual_control_(false),
        controller_net_addr_(controller_net_addr)
      {};

    uint16_t getDeviceID() const;
    // No setter for device ID
    DeviceMode getDeviceMode() const;
    void setDeviceMode(DeviceMode mode);
    DeviceModeFlag getDeviceModeFlags() const;
    void setDeviceModeFlags(DeviceModeFlag mode_flags);
    bool isManualControl() const;
    void setManualControl(bool manual_control);
    NetAddr getControllerNetAddr() const;
    void setControllerNetAddr(NetAddr controller_net_addr);
};

#endif /* _HOT_TUB_PUMP_HPP_ */
