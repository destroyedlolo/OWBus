#include "OWBus.h"

String OWBus::Address::toString(){
	String str;
	for(int i=0; i<8; i++){
		if(addr[i]<16) str += '0';	// Padding si necessaire
		str += String(addr[i], HEX);
	}
	return str;
}

void OWBus::search_reset(){
	ow->reset_search();
}

bool OWBus::search_next(OWBus::Address &a){
	if( ow->search(*a) )
		return true;
	return false;
}

uint8_t OWBus::getDeviceCount(void){
	uint8_t i=0;
	this->search_reset();

	do {
		OWBus::Address a;
		if(!this->search_next(a))
			return i;
		i++;
	} while(1);
}

const char *OWBus::Address::getFamilly(){
	switch(addr[0]){
	case 0x28:
		return "18B20";
	case 0x42:
		return "28EA00";
	default :
		return "Unknown";
	}
}

	/*********
	 * Methodes used in other sub classes
	 * Arduino IDE is not smart enough to handle sources in
	 * sub directories
	 ********/

	/* Devices */
#include <OWBus/OWDevice.h>

bool OWDevice::isParasitePowered(){
	bool r = false;
	bus.getOWTechLayer()->reset();
	bus.getOWTechLayer()->select(*addr);
	bus.getOWTechLayer()->write( this->getOWCommand(READ_POWER_SUPPLY) );
	if(!bus.getOWTechLayer()->read_bit()) r = true;
	bus.getOWTechLayer()->reset();
	return r;
}

	/* Scratchpad */
#include <OWBus/OWScratchpad.h>

uint8_t &OWScratchpad::operator [](uint8_t i){
	if(i < this->getSize())
		return memory[i];
	else
		return fake;
}

bool OWScratchpad::readScratchpad(){
	OneWire *ow = device->getBus().getOWTechLayer();
	
	if(!ow->reset())
		return false;
	
	ow->select(*device->getAddress());
	ow->write( this->device->getOWCommand( OWDevice::OWCommands::READ_SCRATCHPAD ) );

	for(size_t i=0; i<this->getSize(); i++)
		memory[i] = ow->read();

	this->virgin = false;
	return(!!ow->reset());
}

	/* DS18B20 */
#include <OWBus/DS18B20.h>

float DS18B20::readLastTemperature(){
	if(!this->readScratchpad())
		return this->BAD_TEMPERATURE;

	int16_t val = (this->operator[](1) << 8) | this->operator[](0);

	switch(this->operator[](4) & 0x60){	// Clean unused bits
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

	switch(this->operator[](4) & 0x60){
	case 0x00: return 94;	// 9 bits
	case 0x20: return 188;	// 10 bits
	case 0x40: return 375;	// 11 bits
	default:				// 12 bits
		return 750;
	}	
}

uint8_t DS18B20::getResolution(){
	if(virgin && !this->readScratchpad())
		return 0;

	switch(this->operator[](4) & 0x60){
	case 0x00: return 9;
	case 0x20: return 10;
	case 0x40: return 11;
	default:
		return 12;
	}
}

float DS18B20::getTemperature(){
};
