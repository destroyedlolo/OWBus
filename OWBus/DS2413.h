/* Implement DS2413 double PIO
 *
 * 06/11/2017 - L.Faillie - First version
 *
 * Notez-bien :
 * 	- if RESUME (0xA5) needs a custom code here, it needs to be invalidated
 * 		on DS28EA00 side.
 * 	- this probe is currently UNTESTED
 */
#ifndef OWDS2413_H
#define OWDS2413_H 0.0100

#include <OWBus.h>
#include <OWBus/OWDevice.h>

class DS2413 : public OWDevice {
protected:
	uint8_t lastPIOs;

public:
	DS2413( OWBus &abus, OWBus::Address &aa ) : OWDevice( abus, aa ) {}
	DS2413( OWBus &abus, uint64_t aa ) : OWDevice( abus, aa ) {}

	static const uint8_t FAMILLY_CODE = 0x3A;
	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::PIO); }

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
