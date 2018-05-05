#ifndef _HOT_TUB_HEATER_HPP_
#define _HOT_TUB_HEATER_HPP_

#include <devices/core/Device.hpp>
#include <types/device_types.h>

class Heater : public Device {

public:
    Heater( string name = "Heater",
            uint16_t deviceID = DEVICE_ID_NOT_INIT,
            DeviceMode mode = DeviceModeNotInit,
            bool manualControl = false,
            NetAddr controllerNetAddr = 0)
      : Device(name),
        deviceID_(deviceID),
        mode_(mode),
        manualControl_(manualControl),
        controllerNetAddr_(controllerNetAddr)
      {};

    Heater(uint16_t deviceID, NetAddr controllerNetAddr)
      : Device(string("Heater")),
        deviceID_(deviceID),
        mode_(DeviceModeNotInit),
        manualControl_(false),
        controllerNetAddr_(controllerNetAddr)
      {};

    uint16_t getDeviceID() const;
    // No setter for device ID
    DeviceMode getDeviceMode() const;
    void setDeviceMode(DeviceMode mode);
    bool isManualControl() const;
    void setManualControl(bool manualControl);
    NetAddr getControllerNetAddr() const;
    void setControllerNetAddr(NetAddr controllerNetAddr);

    string toString();

private:
    Heater(const Heater&);
    Heater& operator=(const Heater&);

    uint16_t deviceID_;
    DeviceMode mode_;
    bool manualControl_;
    NetAddr controllerNetAddr_;
};

#endif /* _HOT_TUB_HEATER_HPP_ */
