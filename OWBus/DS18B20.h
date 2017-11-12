/* Handle DS18B20 probe
 *
 * 03/11/2017 - L.Faillie - First version
 */

#ifndef OWDS18B20_H
#define OWDS18B20_H	0.0302

#include <OWBus.h>
#include <OWBus/OWDevice.h>
#include <OWBus/OWScratchpad.h>

class DS18B20 : public OWDevice, public OWScratchpad{
public:
	DS18B20( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ), OWScratchpad( this, 9 ) {}
	DS18B20( OWBus &abus, uint64_t aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ), OWScratchpad( this, 9 ) {}

	const float BAD_TEMPERATURE = 9999.0;
	static const uint8_t FAMILLY_CODE = 0x28;
	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::TEMPERATURE | OWDevice::OWCapabilities::EEPROM); }

	unsigned long getConversionDelay();	// Delay needed for the conversion
	uint8_t getResolution();			// return 0 in case of error
	bool setResolution(uint8_t resolution=12);		// if < 9, set to 9, if > 12, set to 12

	enum SCRATCHPAD_INDEX {
		TEMPERATURE_LSB=0, TEMPERATURE_MSB,
		HIGH_ALARM_TEMPERATURE, LOW_ALARM_TEMPERATURE,
		CONFIGURATION, SCRATCHPAD_CRC=8
	};
	virtual bool isValidScratchpad(){	// Verify scratchpad CRC
		return( OneWire::crc8(this->getScratchpadMemory(), 8) == this->operator [](SCRATCHPAD_INDEX::SCRATCHPAD_CRC) );
	}
	virtual bool writeScratchpad();	// Not, it is not copied to the EEPROM
	virtual void forceInvalidScratchpad(){ this->operator[](DS18B20::SCRATCHPAD_INDEX::SCRATCHPAD_CRC) = OneWire::crc8(this->getScratchpadMemory(), 8) ? 0:255; };

		/* if parasite == true, force the bus to be high during the conversion
		 * it's mandatory for parasite-powered probes
		 * it's advisable for externally powered probes to ensure strong
		 * power supply.
		 */
	bool launchTemperatureAcquisition( bool parasite=true );	// Launch temperature acquisition
	float readLastTemperature();	// Read current scratchpad value (get the temperature without requesting a new acquisition
	float getTemperature( bool parasite=true );	// Launch a conversion and read the temperature
};

#endif
