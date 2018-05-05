#ifndef _HOT_TUB_PUMP_HPP_
#define _HOT_TUB_PUMP_HPP_

#include <types/device_types.h>

class Pump : public Device {

public:
    Pump( string name = "Pump",
          uint16_t deviceID = DEVICE_ID_NOT_INIT,
          DeviceMode mode = DeviceModeNotInit,
          DeviceModeFlag modeFlags = DeviceModeFlagNone,
          bool manualControl = false,
          NetAddr controllerNetAddr = 0)
      : Device(name),
        deviceID_(deviceID),
        mode_(mode),
        modeFlags_(modeFlags),
        manualControl_(manualControl),
        controllerNetAddr_(controllerNetAddr)
      {};

    Pump(string name, uint16_t deviceID, NetAddr controllerNetAddr)
      : Device(string(name)),
        deviceID_(deviceID),
        mode_(DeviceModeNotInit),
        modeFlags_(DeviceModeFlagNone),
        manualControl_(false),
        controllerNetAddr_(controllerNetAddr)
      {};

    uint16_t getDeviceID() const;
    // No setter for device ID
    DeviceMode getDeviceMode() const;
    void setDeviceMode(DeviceMode mode);
    DeviceModeFlag getDeviceModeFlags() const;
    void setDeviceModeFlags(DeviceModeFlag modeFlags);
    bool isManualControl() const;
    void setManualControl(bool manualControl);
    NetAddr getControllerNetAddr() const;
    void setControllerNetAddr(NetAddr controllerNetAddr);

    string toString();

private:
    Pump(const Pump&);
    Pump& operator=(const Pump&);

    uint16_t deviceID_;
    DeviceMode mode_;
    DeviceModeFlag modeFlags_;
    bool manualControl_;
    NetAddr controllerNetAddr_;
};

#endif /* _HOT_TUB_PUMP_HPP_ */
