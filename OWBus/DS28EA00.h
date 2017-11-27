/* Handle DS28EA00 probe
 *
 * 04/11/2017 - L.Faillie - First version
 */

#ifndef OWDS28EA00_H
#define OWDS28EA00_H 0.0200

#include <OWBus.h>
#include <OWBus/DS18B20.h>
#include <OWBus/DS2413.h>

	/* Fortunately, DS28EA00 is a DS18B20 in temperature acquisition point of view */
class DS28EA00 : public DS18B20, public DS2413 {
public:
	DS28EA00( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : DS18B20( abus, aa, aname ), DS2413(abus, aa) {}
	DS28EA00( OWBus &abus, uint64_t aa, const char *aname=NULL ) : DS18B20( abus, aa, aname ), DS2413(abus, aa) {}

	virtual uint8_t getOWCommand( OWDevice::OWCommands c ){
		if( c==OWDevice::OWCommands::PIO_WRITE )
			return 0xa5;
		return DS18B20::getOWCommand(c);
	}
	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::TEMPERATURE | OWDevice::OWCapabilities::PIO | OWDevice::OWCapabilities::TEMPERATURE_ALARM ); }

	static const uint8_t FAMILY_CODE = 0x42;
};

#endif
