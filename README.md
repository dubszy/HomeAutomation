# Home Automation

## Abstract
This project is a work in progress that attempts to explore the possibilities of home automation.

## Scope
Currently, this project is a custom implementation of home automation. It makes use of the Raspberry Pi, Arduino, and
various sensors and does not include support for any type of smart home retail products. Support for such products may
be included in the future, but that is not within the current scope of this project.

## Definitions
- HA: Home Automation
- Node: A device with processing power that can run effectively independently on the HA network (note that this project
will make use of Node.JS and that this definition does not apply to Node.JS).

## Requirements
This project makes use of the following other repos:
- [Drivers](https://www.github.com/dubszy/drivers)
- [Logger](https://www.github.com/dubszy/Logger)
- [PropertiesManager](https://www.github.com/dubszy/PropertiesManager)

## Project Structure
#### build/
Build and deployment scripts

#### res/
Resources

#### src/
Source code
##### src/devices/
Appliance-side servers and clients (see Components - Appliance-Side Servers/Clients)
##### src/ha_node/
HANode daemon (see Components - HANode)
##### src/sandbox/
Sandbox folder for testing ideas
##### src/types/
C/C++ types
##### src/utils/
Utility daemons/applications (see Components - * Utility)

## Components
This project is made up of the following components:

### Appliance-Side Servers/Clients
Programs that run on HA-enabled devices to communicate with the rest of the HA network. Located in src/devices

### HANode
Daemon which must run on every IP-enabled device that is part of the HA network. This daemon identifies to the HA
network which node the device it's running on is, and what non-IP-enabled nodes are connected to it. For example, a
Raspberry Pi with an Arduino connected to it could be identified as two separate nodes. In this case, the Pi would run
HANode and identify that there is another node connected to it via serial or USB. For more extensive documentation, see
the README in src/ha_node.

### Device Manager Utility
Discovery service for devices on the HA network. This is used when deploying new code to the network-at-large or to
individual devices. Located in src/utils/DeviceManager