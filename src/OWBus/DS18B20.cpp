/* DS18B20 specifics
 */
#include <OWBus/DS18B20.h>


bool DS18B20::writeScratchpad(){
	OneWire *ow = this->getBus().getOWTechLayer();
	if(!ow->reset())
		return false;

	ow->write( this->getOWCommand( OWDevice::OWCommands::WRITE_SCRATCHPAD ) );
	ow->write( this->operator[](DS18B20::SCRATCHPAD_INDEX::HIGH_ALARM_TEMPERATURE) );
	ow->write( this->operator[](DS18B20::SCRATCHPAD_INDEX::LOW_ALARM_TEMPERATURE) );
	ow->write( this->operator[](DS18B20::SCRATCHPAD_INDEX::CONFIGURATION) );

	this->operator[](DS18B20::SCRATCHPAD_INDEX::SCRATCHPAD_CRC) = OneWire::crc8(this->getScratchpadMemory(), 8);	// Force a valid scratchpad
	return(ow->reset());
}

float DS18B20::readLastTemperature(){
	if(!this->readScratchpad() || !this->isValidScratchpad())
		return this->BAD_TEMPERATURE;

	int16_t val = (this->operator[](DS18B20::SCRATCHPAD_INDEX::TEMPERATURE_MSB) << 8) | this->operator[](DS18B20::SCRATCHPAD_INDEX::TEMPERATURE_LSB);

	switch(this->operator[](DS18B20::SCRATCHPAD_INDEX::CONFIGURATION) & 0x60){	// Clean unused bits
	case 0x00: val &= ~7;	// 9 bits
		break;
	case 0x20: val &= ~3;	// 10 bits
		break;
	case 0x40: val &= ~1;	// 11 bits
		break;
	default :				// 12 bits
		break;
	}

	return val/16.0;
}

unsigned long DS18B20::getConversionDelay(){
	if(virgin && !this->readScratchpad())
		return 1000;	// By default, we will wait for a second

	switch(this->operator[](DS18B20::SCRATCHPAD_INDEX::CONFIGURATION) & 0x60){
	case 0x00: return 94;	// 9 bits
	case 0x20: return 188;	// 10 bits
	case 0x40: return 375;	// 11 bits
	default:				// 12 bits
		return 750;
	}	
}

uint8_t DS18B20::getResolution(){
	if(virgin && !this->readScratchpad())	// read scratchpad if not already done
		return 0;

	switch(this->operator[](DS18B20::SCRATCHPAD_INDEX::CONFIGURATION) & 0x60){
	case 0x00: return 9;
	case 0x20: return 10;
	case 0x40: return 11;
	default:
		return 12;
	}
}

bool DS18B20::setResolution(uint8_t v){
	if(v < 9) v = 9;	// argument conformance
	if(v > 12) v = 12;

	uint8_t c = this->getResolution();	// refresh the scratchpad if needed as well
	if(!c)	// Unable to read the current configuration
		return false;
	if(v == c)	// Already to good value
		return true;

	c = this->operator[](DS18B20::SCRATCHPAD_INDEX::CONFIGURATION);	// Byte storing the resolution
	c &= ~0x60;
	switch( v ){	// Nothing to do for 9bits resolution
	case 10:
		c |= 0x20; break;
	case 11:
		c |= 0x40; break;
	case 12:
		c |= 0x60; break;
	}
	this->operator[](4) = c;

	return this->writeScratchpad();
}

bool DS18B20::launchTemperatureAcquisition(bool parasite){
	OneWire *ow = getBus().getOWTechLayer();
	
	if(!ow->reset())
		return false;
	
	ow->select(*this->getAddress());
	ow->write( this->getOWCommand( OWDevice::OWCommands::CONVERT_T ), parasite );
	return true;
}

float DS18B20::getTemperature(bool parasite){
	if(!this->launchTemperatureAcquisition( parasite ))
		return this->BAD_TEMPERATURE;
	delay( this->getConversionDelay() );
	return this->readLastTemperature();
};

