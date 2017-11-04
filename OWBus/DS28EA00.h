/* Handle DS28EA00 probe
 *
 * 04/11/2017 - L.Faillie - First version
 */

#ifndef OWDS28EA00_H
#define OWDS28EA00_H 0.0100

#include <OWBus.h>
#include <OWBus/DS18B20.h>

	/* Fortunately, DS28EA00 is a DS18B20 in temperature acquisition point of view */
class DS28EA00 : public DS18B20 {
public:
	DS28EA00( OWBus &abus, OWBus::Address &aa ) : DS18B20( abus, aa ) {}
	DS28EA00( OWBus &abus, uint64_t aa ) : DS18B20( abus, aa ) {}

	static const uint8_t FAMILLY_CODE = 0x42;
};

#endif
