#include "OWBus.h"

String OWBus::Addr2String( OWBus::OWAddress &addr ){
}

void OWBus::search_reset(){
	ow->reset_search();
}

bool OWBus::search_next(OWBus::OWAddress &a){
	if( ow->search(a) )
		return true;
	return false;
}

uint8_t OWBus::getDeviceCount(void){
	uint8_t i=0;
	this->search_reset();

	do {
		OWBus::OWAddress a;
		if(!this->search_next(a))
			return i;
		i++;
	} while(1);
}
