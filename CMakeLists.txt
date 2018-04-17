cmake_minimum_required(VERSION 3.10)
project(HomeAutomation)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
include_directories(../drivers/src)
include_directories(../Logger/src/c++)
include_directories(../PropertiesManager/src)

### Daemons and Utilities ###

# HANode daemon
add_executable(hanoded
        src/ha_node/HANode.cpp)

# Device Manager
add_executable(DeviceManager
        src/utils/DeviceManager/DeviceManager.cpp
        src/utils/DeviceManager/IPDeviceManager.cpp
        src/utils/DeviceManager/SerialDeviceManager.cpp)

### Appliances ###

## Hot Tub ##
# Client
add_executable(HotTubClient
        src/devices/appliances/hot-tub/client/client.cpp)
# Server
add_executable(HotTubServer
        src/devices/appliances/Appliance.cpp
        src/devices/appliances/hot-tub/server/HotTub.cpp
        src/devices/appliances/hot-tub/subdevices/Heater.cpp
        src/devices/appliances/hot-tub/subdevices/Pump.cpp
        src/devices/appliances/hot-tub/subdevices/Blower.cpp)
