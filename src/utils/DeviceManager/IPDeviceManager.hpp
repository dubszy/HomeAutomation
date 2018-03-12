#include <core/IPDevice.hpp>
#include "DeviceManager.hpp"

typedef struct {
    NetAddr net_addr;
    IPDevice *device;
} IPDeviceMap;

class IPDeviceManager : DeviceManager {

    protected:
        static IPDeviceManager sharedIPDeviceManager;

    public:
        IPDeviceManager(uint32_t maxDevices) : DeviceManager(maxDevices) {}

        static IPDeviceManager getSharedIPDeviceManager();
        int addDevice(IPDevice *device, NetAddr addr);
        int removeDevice(NetAddr addr);
        IPDevice *getDevice(NetAddr addr);

        IPDeviceMap *discoverDevicesOnLAN();

        IPDevice *ping();
};
