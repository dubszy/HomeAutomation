#ifndef _APPLIANCE_HPP_
#define _APPLIANCE_HPP_

#include <devices/core/Device.hpp>

class Appliance : public Device {

private:
    Appliance(const Appliance&);
    Appliance& operator=(const Appliance&);

public:
    Appliance(string name) : Device(name) {};
};

#endif /* _APPLIANCE_HPP_ */