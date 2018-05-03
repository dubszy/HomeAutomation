#include <cstdio>

#include "DeviceManager.hpp"

DeviceManager *DeviceManager::getSharedDeviceManager() {
    if (DeviceManager::sharedDeviceManager == nullptr) {
        DeviceManager::sharedDeviceManager = new DeviceManager(65536);
    }
    return DeviceManager::sharedDeviceManager;
}

int DeviceManager::addDevice(Device *device, NetAddr addr) {
    if (device == nullptr) {
        printf("Device to add cannot be null\n");
        return -1;
    }

    if (!addr) {
        printf("Network address cannot be null or 0\n");
        return -2;
    }

    if (devices.size() == maxDevices - 1) {
        printf("Maximum devices for this device manager instance is %d\n", maxDevices);
        return -3;
    }

    if (getDevice(addr) != nullptr) {
        printf("Device with network address %d already added\n", addr);
        return -4;
    }

    devices[nextDeviceIndex];
    nextDeviceIndex++;
    return (nextDeviceIndex - 1);
}

int DeviceManager::removeDevice(NetAddr addr) {
    for (int i = 0; i < maxDevices; i++) {
        if (devices.at(i).net_addr == addr) {
            devices.at(i).net_addr = 0;
            devices.at(i).device = nullptr;
            for (int j = i; j < (maxDevices - (i + 1)); j++) {
                devices[j] = devices[j + 1];
            }
            devices.at(maxDevices - 1).net_addr = 0;
            devices.at(maxDevices - 1).device = nullptr;
            return i;
        }
    }
    printf("Device with network address %d was not found\n", addr);
    return -1;
}

Device *DeviceManager::getDevice(NetAddr addr) {
    for (unsigned long i = 0; i < maxDevices; i++) {
        if (devices.at(i).net_addr == addr) {
            return devices.at(i).device;
        }
    }
    return nullptr;
}
