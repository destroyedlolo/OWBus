/* Handle DS2406 PIO chip
 *
 * 09/11/2017 - L.Faillie - First version
 */
#ifndef OWDS2406_H
#define OWDS2406_H	0.0302

#include <OWBus.h>
#include <OWBus/OWDevice.h>

class DS2406 : public OWDevice {
private:

	/* Channel control byte 1 : (2 is always 0xff)
	 * -------------------------------------------
	 * ALR = 1 clear both latches, 0 keep them unchanged
	 * TOG = 0 (else toggling between read and write)
	 * IM = 0 writing, 1 reading
	 * IC = 0 if only single channel / 0 = async / 1 = sync (both updated simultaneously)
	 * CHS0 = PIO.A
	 * CHS1 = PIO.B
	 * CRC1/0 = control CRC generation (0 = none)
	 */
	union {
		struct {
			unsigned int crc : 2;	// 0x01/02
			unsigned int chs_A : 1;	// 0x04
			unsigned int chs_B : 1;	// 0x08
			unsigned int ic : 1;	// 0x10
			unsigned int im : 1;	// 0x20
			unsigned int tog : 1;	// 0x40
			unsigned int alr : 1;	// 0x80
		} bits;
		uint8_t byte;

		void clear(){ this->byte = 0; }
		bool send( bool reset=true );	// reset = true if we don't need the bus afterward

	} ChannelControl;

	/* Channel info byte :
	 * -------------------
	 * 1/0: PIO.B/PIO.A flip-flop (0 = conducting)
	 * 3/2: PIO.B/PIO.A sensed reading
	 * 5/4: PIO.B/PIO.A latches
	 * 6: 0 = A only, 1 = B present
	 * 7:  0 = parasite, 1 = power 
	 */
	union {
		struct {
			unsigned int flipflopA : 1;	// 0x01
			unsigned int flipflopB : 1;	// 0x02
			unsigned int sensedA : 1;	// 0x04
			unsigned int sensedB : 1;	// 0x08
			unsigned int latchA : 1;	// 0x10
			unsigned int latchB : 1;	// 0x20
			unsigned int channels : 1;	// 0x40
			unsigned int supply : 1;	// 0x80
		} bits;
		uint8_t byte;
	} ChannelInfo;
	bool isChannelInfoValide;

	/* STATUS Memory
	 * -------------
	 * 0: 
	 * 	- WP? : Write protect a page
	 * 	- BM? : 0 = used page
	 * 1-4: Page redirection
	 * 5: set to 0x00
	 * 6: 0x00 on a 2406, if 0x00 on a 2407 = hidden mode
	 * 7:
	 * 	CSS0/4 : Condition search to be used with 0xEC
	 * 		CSS4 = B
	 * 		CSS3 = A (if all CSS4/3/0 set to 0, always reply to a search)
	 * 		CSS2/1 = source
	 * 			0b01 = latch (stat change detect - reset when ALR set to 1)
	 * 			0b10 = transitor status (0 = on)
	 * 			0b11 = PIO (actual value)
	 * 		CSS0 = polarity
	 * 	Flip-flop A/B : 0 conducting, 1 float (*)
	 * 	Supply : 0 = parasite, 1 = power (*)
	 *
	 * (*) accessible through the Channel Access function
	 */
	union {
		struct {
				/* Condition search parameters */
			unsigned int polarity : 1;
			unsigned int sources : 2;
			unsigned int channelA : 1;
			unsigned int channelB : 1;

				/* copy of ChannelInfo */
			unsigned int flipflopA : 1;
			unsigned int flipflopB : 1;
			unsigned int supply : 1;
		} bits;
		uint8_t byte;
	} StatusMemory;
	bool isStatusMemoryValide;

public:
	enum Source {	// Condition search sources
		LATCH = 1, FLIPFLOP, PIO 
	};

	DS2406( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) { this->clear(); }
	DS2406( OWBus &abus, uint64_t aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) { this->clear(); }

#ifdef IMPLEMENT_BITFIELD_TEST
		/* Check the order of bits fields 
		 *	*MUST* return 128
		 */
	bool checkArchitecture( void ){
		ChannelControl.byte = 0;
		ChannelControl.bits.alr = true;
		return (ChannelControl.byte == 128);
	}
#endif

	static const uint8_t FAMILLY_CODE = 0x12;

	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::EEPROM | OWDevice::OWCapabilities::PIO | OWDevice::OWCapabilities::PIO_ALARM ); }


	/* 
	 * DS2406's
	 */
protected:
	void clear( void ){	/* Clear all fields */
		ChannelControl.clear();
		isChannelInfoValide = false;
		isStatusMemoryValide = false;
	}

	bool doChannelAccess( bool reset=true );	// reset = true if we don't need the bus afterward

public:
	bool hasPIOB( bool reload=false );
	virtual bool isParasitePowered( bool reload=false );

		/* get FlipFlop (transistors' control) */
	bool getFlipFlopA( bool reload=false );
	bool getFlipFlopB( bool reload=false );

		/* get latches */
	bool getLatcheA( bool reload=false );
	bool getLatcheB( bool reload=false );

	/* Access functions.
	 * Try to be as compatible as possible with other PIO capables chips
	 */

		/* sense PIOs */
	bool getPIOA( bool reload=false );
	bool getPIOB( bool reload=false );
	bool arePIOsValid( ) { return isChannelInfoValide; }

		/* set PIOs */
	bool setPIOA( bool val, bool portB = false );	// if true, PortB select B
	bool setPIOB( bool val ) { this->setPIOA( val, true ); }

		/* Compatibilities */
	enum PIObitsvalue { PIOAbit=1, PIOBbit=2 };
	uint8_t readPIOs( bool reset=true );	// Read PIOs
	bool writePIOs( uint8_t );	// Write PIOs
};
#endif
