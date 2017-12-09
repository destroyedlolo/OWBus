/* DS2413 specifics
 *
 *	ports are active when LOW.
 */
#include <OWBus/DS2413.h>

bool DS2413::writePIOs( uint8_t val ){
	OneWire *ow = getBus().getOWTechLayer();

	if(!ow->reset())
		return false;
	
	bool res = true;
	ow->select(*this->getAddress());
	ow->write( this->getOWCommand( OWDevice::OWCommands::PIO_WRITE ) );
	ow->write(val);		// Write the value
	ow->write(~val);	// and it's complement

	if( ow->read() != 0xaa )	// chip's feedback
		res = false;	// Failure
	
	ow->reset();
	return res;
}

uint8_t DS2413::readPIOs(){
	OneWire *ow = getBus().getOWTechLayer();

	if(!ow->reset())
		return false;
	
	ow->select(*this->getAddress());
	ow->write( this->getOWCommand( OWDevice::OWCommands::PIO_READ ) );
	
	this->lastPIOs = ow->read();
	ow->reset();

	return( this->lastPIOs );
}

bool DS2413::getPIOA( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;
	return(val & 1);
}

bool DS2413::getPIOB( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;
	return(val & 4);
}

bool DS2413::getFlipFlopA( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;
	return(val & 2);
}

bool DS2413::getFlipFlopB( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;
	return(val & 8);
}

void DS2413::setPIOA( bool val ){
	if(!this->arePIOsValid())	// Refresh cache if not valide
		this->readPIOs();

	this->writePIOs( 
		(val ? DS2413::PIObitsvalue::PIOAbit : 0) |
		( this->getFlipFlopB() ? DS2413::PIObitsvalue::PIOBbit : 0 )
	);

	this->lastPIOs = 0;	// Make cache invalide
}

void DS2413::setPIOB( bool val ){
	if(!this->arePIOsValid())	// Refresh cache if not valide
		this->readPIOs();

	this->writePIOs( 
		(val ? DS2413::PIObitsvalue::PIOBbit : 0) |
		( this->getFlipFlopA() ? DS2413::PIObitsvalue::PIOAbit : 0 )
	);

	this->lastPIOs = 0;	// Make cache invalide
}

bool DS2413::arePIOsValid( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;

	return((val & 0x0f) == (~((val & 0xf0) >> 4) & 0x0f ));
}
