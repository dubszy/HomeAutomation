#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_

#include <stdlib.h>

#include <core/Device.hpp>
#include <types/NetAddr.h>

typedef struct {
    NetAddr net_addr;
    Device *device;
} DeviceMap;

class DeviceManager {

    protected:
        uint32_t maxDevices;
        uint32_t nextDeviceIndex;
        static DeviceManager sharedDeviceManager;
        DeviceMap devices[maxDevices];

    public:
        DeviceManager(uint32_t maxDevices) {
            this->maxDevices = maxDevices;
            this->nextDeviceIndex = 0;
            devices = malloc(maxDevices*sizeof(DeviceMap));
        }

        static DeviceManager getSharedDeviceManager();
        int addDevice(Device *device, NetAddr addr);
        int removeDevice(NetAddr addr);
        Device *getDevice(NetAddr addr);
};

#endif /* _DEVICE_MANAGER_H_ */
