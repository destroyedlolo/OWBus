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
#define OWDS2413_H 0.0300

#include <OWBus.h>
#include <OWBus/OWDevice.h>

class DS2413 : virtual public OWDevice {
protected:
	uint8_t lastPIOs;

public:
	DS2413( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) {}
	DS2413( OWBus &abus, uint64_t aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) {}

	static const uint8_t FAMILY_CODE = 0x3A;
	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::PIO); }

		/* Both PIOs are read or written at the same time */
	enum PIObitsvalue { PIOAbit=1, PIOBbit=2 };
	enum PIOmask { PIONONEmask=0xff, PIOAmask=0xfe, PIOBmask=0xfd };

		/******
		 * Sense PIOs
		 ******
		 * If no argument is provided, takes the last readPIOs() value
		 */
	bool getPIOA( uint8_t val = (uint8_t)-1 );
	bool getPIOB( uint8_t val = (uint8_t)-1 );

		/******
		 * Get PIOs output status
		 ******
		 * If no argument is provided, takes the last readPIOs() value
		 */		
	bool getFlipFlopA( uint8_t val = (uint8_t)-1 );	// To be consistent with DS2406
	bool getFlipFlopB( uint8_t val = (uint8_t)-1 );

	bool getLatcheA( uint8_t val = (uint8_t)-1 ){ return this->getFlipFlopA(val); }	// To be consistent with the datasheet
	bool getLatcheB( uint8_t val = (uint8_t)-1 ){ return this->getFlipFlopB(val); };

		/******
		 * set PIOs
		 ******
		 * Notez-bien : current latches values are read if invalid.
		 * If you're not sure of current cached value, use readPIOs()
		 */
	void setPIOA( bool val );
	void setPIOB( bool val );

	bool arePIOsValid( uint8_t val = (uint8_t)-1 );
	uint8_t readPIOs( void );	// Read PIOs
	bool writePIOs( uint8_t );	// Write PIOs
};

#endif
