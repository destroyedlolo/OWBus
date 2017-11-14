/* Handle DS2406 PIO chip
 *
 * 09/11/2017 - L.Faillie - First version
 */
#ifndef OWDS2406_H
#define OWDS2406_H	0.0100

#include <OWBus.h>
#include <OWBus/OWDevice.h>

/* As per https://www.maximintegrated.com/en/app-notes/index.mvp/id/5856
 *
 * Channel control byte :
 * TOG = 0 (else toggling between read and write)
 * IM = 0 writing, 1 reading
 * IC = 0 if only single channel
 * CHS0 = PIO.A
 * CHS1 = PIO.B
 *
 * STATUS Memory (addr 7)
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
 * 		CSS0 = value
 * 	Flip-flop A/B : 0 conducting, 1 float (*)
 * 	Supply : 0 = parasite, 1 = power (*)
 *
 * (*) accessible through the Channel Access function
 */

class DS2406 : public OWDevice {
public:
	DS2406( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) {}
	DS2406( OWBus &abus, uint64_t aa, const char *aname=NULL ) : OWDevice( abus, aa, aname ) {}

	static const uint8_t FAMILLY_CODE = 0x12;

	virtual uint64_t getOWCapability(){ return(OWDevice::OWCapabilities::EEPROM | OWDevice::OWCapabilities::PIO); }

};

#endif
