/* Handle DS18B20 probe
 *
 * 03/11/2017 - L.Faillie - First version
 */

#ifndef OWDS18B20_H
#define OWDS18B20_H

#include <OWBus.h>
#include <OWBus/OWDevice.h>
#include <OWBus/OWScratchpad.h>

class DS18B20 : public OWDevice, public OWScratchpad{
public:
	DS18B20( OWBus &abus, OWBus::Address &aa ) : OWDevice( abus, aa ), OWScratchpad( this, 9 ) {}
	DS18B20( OWBus &abus, uint64_t aa ) : OWDevice( abus, aa ), OWScratchpad( this, 9 ) {}

	const float BAD_TEMPERATURE = 9999.0;

	float readLastTemperature();	// Read current scratchpad value
};

#endif
