#include <stdio.h>

#include "DeviceManager.hpp"

static DeviceManager DeviceManager::getSharedDeviceManager() {
    if (!sharedDeviceManager) {
        sharedDeviceManager = new DeviceManager(65536);
    }
    return sharedDeviceManager;
}

int DeviceManager::addDevice(Device *device, NetAddr addr) {
    if (device == NULL) {
        printf("Device to add cannot be null\n");
        return -1;
    }

    if (!addr) {
        printf("Network address cannot be null or 0\n");
        return -2;
    }

    if (devices[maxDevices - 1] != NULL) {
        printf("Maximum devices for this device manager instance is %d\n", maxDevices);
        return -3;
    }

    if (getDevice(addr) != NULL) {
        printf("Device with network address %d already added\n", addr);
        return -4;
    }

    devices[nextDeviceIndex];
    nextDeviceIndex++;
    return (nextDeviceIndex - 1);
}

int DeviceManager::removeDevice(NetAddr addr) {
    for (int i = 0; i < maxDevices; i++) {
        if (devices[i].net_addr == addr) {
            devices[i] == NULL;
            for (int j = i; j < (maxDevices - (i + 1)); j++) {
                devices[j] = devices[j + 1];
            }
            devices[(maxDevices - 1)] = NULL;
            return i;
        }
    }
    printf("Device with network address %d was not found\n", addr);
    return -1;
}

Device *DeviceManager::getDevice(NetAddr addr) {
    for (int i = 0; i < maxDevices; i++) {
        if (devices[i].net_addr == addr) {
            return devices[i].device;
        }
    }
    return NULL;
}
