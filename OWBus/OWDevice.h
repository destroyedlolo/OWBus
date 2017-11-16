/* Generic OneWire device handling
 *
 * 02/11/2017 - L.Faillie - First version
 */
#ifndef OWDEVICE_H
#define OWDEVICE_H 0.0403

#include <OWBus.h>

class OWDevice {
public:
private:
	OWBus &bus;
	OWBus::Address addr;
	String name;

public:
	OWDevice( OWBus &abus, OWBus::Address &aa, const char *aname=NULL ) : bus(abus), addr(aa){ if(aname) name=aname; };
	OWDevice( OWBus &abus, uint64_t aa, const char *aname=NULL ) : bus(abus), addr(aa){ if(aname) name=aname; };

		/* This enum contains all (useful) known 1-w commands.
		 * Commands directly handled by OneWire library are omitted.
		 * getOWCommand() will convert to a particular chip code if it
		 * is not the same as this table.
		 * Notez-bien : this list contains ALL code, not only for a particular
		 * chip.
		 */
	enum OWCommands {
		CONVERT_T=0x44,
		WRITE_SCRATCHPAD=0x4e, READ_SCRATCHPAD=0xbe,
		COPY_SCRATCHPAD=0x48, RECALL_E2=0xb8,
		PIO_READ=0xf5, PIO_WRITE=0x5a,
		WRITE_STATUS=0x55, READ_STATUS=0xAA,
		RESUME=0xa5, READ_POWER_SUPPLY=0xB4,
		CHANNEL_ACCESS=0xf5
	};
	virtual uint8_t getOWCommand( OWCommands c ){ return c; }

	enum OWCapabilities {	// caution : 1 bit per capability
		TEMPERATURE = 1, PIO = 2, EEPROM = 4,
		TEMPERATURE_ALARM = 8, PIO_ALARM = 0x10
	};
	virtual uint64_t getOWCapability(){ return 0; }

	OWBus &getBus(){ return bus; }
	OWBus::Address &getAddress(){ return addr; }
	const char *getFamilly(){ return addr.getFamilly(); }
	virtual bool isParasitePowered();
	String getName();
};

#endif
