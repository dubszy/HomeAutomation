#ifndef _HOT_TUB_BLOWER_HPP_
#define _HOT_TUB_BLOWER_HPP_

#include <devices/core/Device.hpp>
#include <types/device_types.h>

class Blower : public Device {

public:
    Blower( string name = "Blower",
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

    Blower(uint16_t deviceID, NetAddr controllerNetAddr)
      : Device(string("Blower")),
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
    Blower(const Blower&);
    Blower& operator=(const Blower&);

    uint16_t deviceID_;
    DeviceMode mode_;
    bool manualControl_;
    NetAddr controllerNetAddr_;
};

#endif /* _HOT_TUB_BLOWER_HPP_ */
