# OWBus

OWBus is a library that handles several type of Maxim's 1-Wire probes for Arduino and ESP8266

OWBus requires the OneWire library which can be found at
http://www.pjrc.com/teensy/td_libs_OneWire.html

Have a look on provided examples as well as .h files for additionnal informations.

Interfaces implementation :

## OWBus (OWBus.h)

Abstracts 1-wire bus and handles
- **search/discovery** of probes on the network
- **launch** parallel temperature conversion
- ...

## OWBus::Address (OWBus.h)

Abstracts 1-wire address handling
- validation
- **to string** conversion
- chip family identification
- ...

## OWDevice (OWBus/OWDevice.h)

Generic class to handle all probles.
- **1-wire** commands 
- shared capabilities (parasite-powered, ...)
- ...

## OWScratchpad (OWBus/OWScratchpad)

Generic scratchpad handling.
- **read**/**write**
- ...

## DS18B20
Handles well known ds18b20 temperature probe.
- synchronous and asynchronous temperature conversion
- precision
- conversion timings

### Features Support
- Temperature : **Tested ok**
- Precision : **Tested ok**
- Alarms : *To be done*
- EEprom : *To be done*

## DS28EA00
Handles ds28ea00 temperature probe as well as its PIOs.

### Features Support
- Temperature : **Tested ok**
- Precision : **Tested ok**
- Alarms : *To be done*
- EEprom : *To be done*
- Chain : *To be done*
- PIO writing : **Tested ok**
- PIO reading : **Tested ok**

## DS2413
Handles ds2413 dual simple PIOs

### Features Support
- PIO writing : **implemented but untested**
- PIO reading : **implemented but untested**

## DS2406
Handle ds2406 and 07 (which is dectected as DS2406)

### Important Note :
Before working with a new device, please test
**NumberOfProbes.ino** with *Test bits ordering* enabled.
It must output "*Arch Ok*" otherwise open a bug report.

### Features Support
- EEPROM Writing : **Not implemented** and will probably remain
- EEPROM Reading : *To be done* (or not, I'm not sure there is a need here)
- Alarms : *To be done*
- PIO writing : **Tested ok**
- PIO reading : **Tested ok**

