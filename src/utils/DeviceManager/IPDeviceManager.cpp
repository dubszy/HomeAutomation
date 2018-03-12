#include "IPDeviceManager.hpp"

static IPDeviceManager IPDeviceManager::getSharedIPDeviceManager() {
    if (!sharedIPDeviceManager) {
        sharedIPDeviceManager = new IPDeviceManager(65536);
    }
    return sharedIPDeviceManager;
}

int IPDeviceManager::addDevice(IPDevice *device, NetAddr addr) {
    return DeviceManager::addDevice(device, addr);
}

int IPDeviceManager::removeDevice(NetAddr addr) {
    return DeviceManager::removeDevice(addr);
}

IPDevice *IPDeviceManager::getDevice(NetAddr addr) {
    return (IPDevice *)DeviceManager::getDevice(addr);
}

IPDeviceMap *IPDeviceManager::discoverDevicesOnLAN() {
    IPDeviceMap lanDevices[65536] = malloc(65536*sizeof(IPDeviceMap));

    return lanDevices;
}

IPDevice *IPDeviceManager::ping() {

}
