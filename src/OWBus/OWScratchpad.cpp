/* Scratchpad handling
 */

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


