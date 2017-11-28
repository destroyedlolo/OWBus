/* Generic 1-wire device handling
 */

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

String OWDevice::getName(){
	if(name)
		return name;
	else
		return this->getAddress().toString();
}

