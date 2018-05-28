#ifndef _SERIAL_DEVICE_MANAGER_HPP_
#define _SERIAL_DEVICE_MANAGER_HPP_

#include <devices/core/SerialDevice.hpp>

#include "DeviceManager.hpp"

typedef struct {
    NetAddr net_addr;
    SerialDevice *device;
} SerialDeviceMap;

class SerialDeviceManager : DeviceManager {

    public:
        SerialDeviceManager(uint32_t maxDevices) : DeviceManager(maxDevices) {}

        static SerialDeviceManager *getSharedSerialDeviceManager();
        int addDevice(SerialDevice *device, NetAddr addr);
        int removeDevice(NetAddr addr);
        SerialDevice *getDevice(NetAddr addr);

        vector<SerialDeviceMap> discoverLocalDevices();
        vector<SerialDeviceMap> discoverDevicesOnLAN();
};

#endif /* _SERIAL_DEVICE_MANAGER_HPP_ */
