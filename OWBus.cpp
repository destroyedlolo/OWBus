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

	/* DS18B20 and DS28EA00 */
#include <OWBus/DS18B20.h>
#include <OWBus/DS28EA00.h>

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

	/* DS28EA00 specifics
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

bool DS2413::arePIOsValid( uint8_t val ){
	if(val == (uint8_t)-1)
		val = this->lastPIOs;

	return((val & 0x0f) == (~((val & 0xf0) >> 4) & 0x0f ));
}

	/* DS2406 specifics
	 *	ports are active when LOW.
	 */
#include <OWBus/DS2406.h>

/* As per https://www.maximintegrated.com/en/app-notes/index.mvp/id/5856 */
bool DS2406::doChannelAccess( bool reset ){
	/* <- reset : do we have to reset the bus when finished */
	OneWire *ow = getBus().getOWTechLayer();
	if(!ow->reset())
		return false;
	
	ow->select(*this->getAddress());
	ow->write( this->getOWCommand( OWDevice::OWCommands::CHANNEL_ACCESS ) );
	ow->write( this->ChannelControl.byte ); // Channel Control Byte 1
	ow->write( 0xff ); // Channel Control Byte 2
	this->ChannelInfo.byte = ow->read();	// Read Channel Info
	this->isChannelInfoValide = true;

	if(reset) ow->reset();

	return true;
}

uint8_t DS2406::readPIOs( bool reset ){	// Update ChannelInfo
	/* <- reset : do we have to reset the bus when finished */
	this->ChannelControl.clear();
	this->ChannelControl.bits.im = true;

	if(!this->doChannelAccess( reset ))
		return 0;

	return ( ((this->ChannelInfo.byte) >> 2) & 0x03 );
}

/* To be done
bool writePIOs( uint8_t );
 */

bool DS2406::hasPIOB( void ){
	if(!this->arePIOsValid())
		this->readPIOs( true );

	return( this->ChannelInfo.bits.channels );
}

bool DS2406::isParasitePowered( void ){
	if(!this->arePIOsValid())
		this->readPIOs( true );

	return( !this->ChannelInfo.bits.supply);
}

bool DS2406::getLatcheA( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.latchA );
}

bool DS2406::getLatcheB( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.latchB );
}

bool DS2406::getPIOA( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.sensedA );
}

bool DS2406::getPIOB( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.sensedB );
}

bool DS2406::setPIOA( bool val, bool portB ){
	this->ChannelControl.clear();
	if(!portB)
		this->ChannelControl.bits.chs_A = true;
	else
		this->ChannelControl.bits.chs_B = true;

	OneWire *ow = getBus().getOWTechLayer();
	this->doChannelAccess( false );
	ow->write_bit( val );
	ow->reset();
	
	return true;
}

	/*
	 * General functions 
	 */
bool OWBus::launchTemperatureAcquisition(bool parasite){
	OneWire *ow = this->getOWTechLayer();
	
	if(!ow->reset())
		return false;
	
	ow->skip();
		// It seems all compatible probes are using the same code
	ow->write( OWDevice::OWCommands::CONVERT_T, parasite );
	return true;
}

const char *OWBus::Address::getFamilly(){
	switch(addr[0]){
	case DS18B20::FAMILLY_CODE :
		return "DS18B20";
	case DS28EA00::FAMILLY_CODE:
		return "DS28EA00";
	case DS2406::FAMILLY_CODE:
		return "DS2406";	/* and DS2407 */
	case DS2413::FAMILLY_CODE:
		return "DS2413";
	default :
		return "Unknown";
	}
}

