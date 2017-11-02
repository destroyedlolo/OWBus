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

	/*********
	 * Methodes used in other sub classes
	 * Arduino IDE is not smart enough to handle sources in
	 * sub directories
	 ********/

	/* Devices */
#include <OWBus/OWDevice.h>

uint8_t &OWDevice::ScrachPad::operator [](uint8_t i){
	if(i < this->getSize())
		return memory[i];
	else
		return fake;
}

bool OWDevice::ScrachPad::read(){
	OneWire *ow = device->getBus().getOWTechLayer();
	
	if(!ow->reset())
		return false;
	
	ow->select(*device->getAddress());

	return true;
}

bool OWDevice::ifConnected(){
}
