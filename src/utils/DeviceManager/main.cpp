#include <utils/DeviceManager/DeviceManager.hpp>
#include <utils/DeviceManager/IPDeviceManager.hpp>
#include <utils/DeviceManager/SerialDeviceManager.hpp>

int main() {
    DeviceManager *deviceManager = DeviceManager::getSharedDeviceManager();
    IPDeviceManager *ipDeviceManager = IPDeviceManager::getSharedIPDeviceManager();
    SerialDeviceManager *serialDeviceManager = SerialDeviceManager::getSharedSerialDeviceManager();
    return 0;
}