cmake_minimum_required(VERSION 3.10)
project(HomeAutomation)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(../drivers/include)
include_directories(../Logger/include)
include_directories(../PropertiesManager/include)

link_directories(../drivers/cmake-build-debug)
link_directories(../Logger/cmake-build-debug)
link_directories(../PropertiesManager/cmake-build-debug)


### Daemons and Utilities ###

# HANode daemon
add_executable(hanoded
        src/ha_node/HANode.cpp)

target_link_libraries(hanoded liblogger.a libPropertiesManager.a)

# Device Manager
add_executable(DeviceManager
        src/utils/DeviceManager/DeviceManager.cpp
        src/utils/DeviceManager/IPDeviceManager.cpp
        src/utils/DeviceManager/SerialDeviceManager.cpp)

target_link_libraries(DeviceManager liblogger.a)

### Appliances ###

## Hot Tub ##
# Client
#add_executable(HotTubClient
#        src/devices/appliances/hot-tub/client/client.cpp)
# Server
add_executable(HotTubServer
        src/devices/appliances/hot-tub/subdevices/Heater.cpp
        src/devices/appliances/hot-tub/subdevices/Pump.cpp
        src/devices/appliances/hot-tub/subdevices/Blower.cpp
        src/devices/appliances/Appliance.cpp
        src/devices/appliances/hot-tub/server/HotTub.cpp
        src/devices/appliances/hot-tub/server/main.cpp)

target_link_libraries(HotTubServer liblogger.a libspi.a libonewire.a libds18b20.a)
