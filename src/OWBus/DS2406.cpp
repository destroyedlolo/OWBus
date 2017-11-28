/* DS2406 specifics
 *
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

bool DS2406::hasPIOB( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.channels );
}

bool DS2406::isParasitePowered( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( !this->ChannelInfo.bits.supply);
}

bool DS2406::getFlipFlopA( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.flipflopA );
}

bool DS2406::getFlipFlopB( bool reload ){
	if(reload || !this->arePIOsValid() )
		this->readPIOs( true );

	return( this->ChannelInfo.bits.flipflopB );
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
	this->clear();	// ChannelInfo and StatusMemory will change as well
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


