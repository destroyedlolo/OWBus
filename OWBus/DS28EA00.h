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
	uint8_t lastPIOs;

public:
	DS28EA00( OWBus &abus, OWBus::Address &aa ) : DS18B20( abus, aa ) {}
	DS28EA00( OWBus &abus, uint64_t aa ) : DS18B20( abus, aa ) {}

	static const uint8_t FAMILLY_CODE = 0x42;

		/* Both PIOs are read or written at the same time */
	enum PIObitsvalue { PIOAbit=1, PIOBbit=2 };
	enum PIOmask { PIONONEmask=0xff, PIOAmask=0xfe, PIOBmask=0xfd };

		// If no argument is provided, takes the last readPIOs() value
	bool PIOA( uint8_t val = (uint8_t)-1 );
	bool PIOB( uint8_t val = (uint8_t)-1 );
	bool arePIOsValid( uint8_t val = (uint8_t)-1 );

	uint8_t readPIOs();	// Read PIOs
	bool writePIOs( uint8_t );	// Write PIOs
};

#endif
