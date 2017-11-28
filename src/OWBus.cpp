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

/* Methods needing probes' definition */
#include <OWBus/DS18B20.h>
#include <OWBus/DS28EA00.h>
#include <OWBus/DS2413.h>
#include <OWBus/DS2406.h>

bool OWBus::launchTemperatureAcquisition(bool parasite){
	OneWire *ow = this->getOWTechLayer();
	
	if(!ow->reset())
		return false;
	
	ow->skip();
		// It seems all compatible probes are using the same code
	ow->write( OWDevice::OWCommands::CONVERT_T, parasite );
	return true;
}

const char *OWBus::Address::getFamily(){
	switch(addr[0]){
	case DS18B20::FAMILY_CODE :
		return "DS18B20";
	case DS28EA00::FAMILY_CODE:
		return "DS28EA00";
	case DS2406::FAMILY_CODE:
		return "DS2406";	/* and DS2407 */
	case DS2413::FAMILY_CODE:
		return "DS2413";
	default :
		return "Unknown";
	}
}

