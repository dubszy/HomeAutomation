#include "SerialDeviceManager.hpp"

SerialDeviceManager *SerialDeviceManager::getSharedSerialDeviceManager() {
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

vector<SerialDeviceMap> SerialDeviceManager::discoverLocalDevices() {
    vector<SerialDeviceMap> localDevices = vector<SerialDeviceMap>(1024);

    return localDevices;
}

vector<SerialDeviceMap> SerialDeviceManager::discoverDevicesOnLAN() {
    vector<SerialDeviceMap> lanDevices = vector<SerialDeviceMap>(2048);

    vector<SerialDeviceMap> localDevices = discoverLocalDevices();
    for (int i = 0; i < (localDevices.size() / sizeof(SerialDeviceMap)); i++) {
        lanDevices[i] = localDevices[i];
    }

    return lanDevices;
}
