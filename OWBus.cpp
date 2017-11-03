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

bool OWDevice::ifConnected(){
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
