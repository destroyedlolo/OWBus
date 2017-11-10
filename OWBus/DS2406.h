/* Handle DS2406 PIO chip
 *
 * 09/11/2017 - L.Faillie - First version
 */
#ifndef OWDS2406_H
#define OWDS2406_H	0.0100

#include <OWBus.h>
#include <OWBus/OWDevice.h>

class DS2406 : public OWDevice {
public:
	DS2406( OWBus &abus, OWBus::Address &aa ) : OWDevice( abus, aa ) {}
	DS2406( OWBus &abus, uint64_t aa ) : OWDevice( abus, aa ) {}

	static const uint8_t FAMILLY_CODE = 0x12;
};

#endif
