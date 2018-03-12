#include "SerialDeviceManager.hpp"

static SerialDeviceManager SerialDeviceManager::getSharedSerialDeviceManager() {
    if (!sharedSerialDeviceManager) {
        sharedSerialDeviceManager = new SerialDeviceManager(65536);
    }
    return sharedSerialDeviceManager;
}

int SerialDeviceManager::addDevice(SerialDevice *device, NetAddr addr) {
    return DeviceManager::addDevice(device, addr);
}

int SerialDeviceManager::removeDevice(NetAddr addr) {
    return DeviceManager::removeDevice(addr);
}

SerialDevice *SerialDeviceManager::getDevice(NetAddr addr) {
    return (SerialDevice *)DeviceManager::getDevice(addr);
}

SerialDeviceMap *SerialDeviceManager::discoverLocalDevices() {
    SerialDeviceMap localDevices[65536] = malloc(65536*sizeof(SerialDeviceMap));

    return localDevices;
}

SerialDeviceMap *SerialDeviceManager::discoverDevicesOnLAN() {
    SerialDeviceMap lanDevices[65536] = malloc(65536*sizeof(SerialDeviceMap));

    SerialDeviceMap *localDevices = discoverLocalDevices();
    for (int i = 0; i < (sizeof(localDevices) / sizeof(SerialDeviceMap)); i++) {
        lanDevices[i] = localDevices[i];
    }

    return lanDevices;
}
