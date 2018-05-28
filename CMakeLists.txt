cmake_minimum_required(VERSION 3.6)
project(HomeAutomation)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
# dev include directories
include_directories(../drivers/include)
include_directories(../Logger/include)
include_directories(../PropertiesManager/include)
# build server include directories
include_directories(/home/ha/deploy/drivers/include)
include_directories(/home/ha/deploy/Logger/include)
include_directories(/home/ha/deploy/PropertiesManager/include)

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
        src/utils/DeviceManager/SerialDeviceManager.cpp
        src/utils/DeviceManager/main.cpp)

target_link_libraries(DeviceManager liblogger.a)

### Living Room Server ###
add_executable(livingroomd
        src/server/livingroom/LivingRoom.cpp
        src/server/livingroom/main.cpp)

### Kitchen Server ###
add_executable(kitchend
        src/server/kitchen/Kitchen.cpp
        src/server/kitchen/main.cpp)

### Bedroom Server ###
add_executable(bedroomd
        src/server/bedroom/Bedroom.cpp
        src/server/bedroom/main.cpp)

### Garage Server ###
add_executable(garage
        src/server/garage/Garage.cpp
        src/server/garage/main.cpp)

### Appliances ###

## Hot Tub ##
add_executable(HotTubServer
        src/devices/appliances/hot-tub/subdevices/Heater.cpp
        src/devices/appliances/hot-tub/subdevices/Pump.cpp
        src/devices/appliances/hot-tub/subdevices/Blower.cpp
        src/devices/appliances/Appliance.cpp
        src/devices/appliances/hot-tub/server/HotTub.cpp
        src/devices/appliances/hot-tub/server/main.cpp)

target_link_libraries(HotTubServer liblogger.a libPropertiesManager.a libspi.a libonewire.a libds18b20.a)
