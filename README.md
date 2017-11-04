# OWBus

OWBus is a library that handles several type of Maxim's 1-Wire probes for Arduino and ESP8266

Have a look on provided examples as well as .h files for additionnal informations.

In a user point of vue :

## OWBus (OWBus.h)

Abstracts 1-wire bus and handles
- **search/discovery** of probes on the network
- **launch** parallel temperature conversion
- ...

## OWBus::Address (OWBus.h)

Abstracts 1-wire address handling
- validation
- **to string** conversion
- chip familly identification
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
- ...

## DS28EA00
Handles ds28ea00 temperature probe as well as its PIO.
