#ifndef _HOT_TUB_SERVER_HPP_
#define _HOT_TUB_SERVER_HPP_

#include <devices/core/SPIDevice.hpp>
#include <devices/environment/temperature/thermometers/DS18B20.hpp>

#include <devices/appliances/Appliance.hpp>
#include <types/device_types.h>

#include <devices/appliances/hot-tub/subdevices/Blower.hpp>
#include <devices/appliances/hot-tub/subdevices/Heater.hpp>
#include <devices/appliances/hot-tub/subdevices/Pump.hpp>

#include <logger/c++/Logger.hpp>

/* Device IDs */
#define DEVICE_ID_HEATER    0x0001
#define DEVICE_ID_PUMP1     0x0002
#define DEVICE_ID_PUMP2     0x0003
#define DEVICE_ID_BLOWER    0x0004

typedef struct {
    string name;
    uint8_t *addr;
    float temp_limit;
} ThermometerConfig;

typedef struct {
    string name;
    ThermometerConfig *zone0_config;
    ThermometerConfig *zone1_config;
    ThermometerConfig *zone2_config;
    ThermometerConfig *zone3_config;
} HotTubConfig;

class HotTub : public Appliance {

public:
    HotTub(string name = "Hot Tub", NetAddr netAddr = 0);

    ~HotTub();

    DS18B20 *getZone0Thermo();
    DS18B20 *getZone1Thermo();
    DS18B20 *getZone2Thermo();
    DS18B20 *getZone3Thermo();

    int changeDeviceMode(BasicDeviceInfo device_info);
    int changeHeaterMode(DeviceMode mode);
    int changePump1Mode(DeviceMode mode, DeviceModeFlag mode_flags);
    int changePump2Mode(DeviceMode mode);
    int changeBlowerMode(DeviceMode mode);

    void selfTest();

    string toString();

private:
    HotTub(const HotTub&);
    HotTub& operator=(const HotTub&);

    NetAddr netAddr_;

    SPIDevice *client_; // Arduino

    DS18B20 *zone0_thermo_; // Near heater
    DS18B20 *zone1_thermo_; // Pump 1 side
    DS18B20 *zone2_thermo_; // Pump 2 side
    DS18B20 *zone3_thermo_; // Footwell

    Heater *heater_;
    Pump *pump1_;
    Pump *pump2_;
    Blower *blower_;

    int clientWrite(ClientTransaction txn);
    Transaction clientRead();

    Logger *log = Logger::forClass<HotTub>(LogLevelInfo);
};

#endif /* _HOT_TUB_SERVER_HPP_ */
