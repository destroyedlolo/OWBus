/* Generic OneWire device handling
 *
 * 02/11/2017 - L.Faillie - First version
 */
#ifndef OWDEVICE_H
#define OWDEVICE_H 0.0100

#include <OWBus.h>

class OWDevice {
public:
	class ScrachPad {
		uint8_t fake;

	protected:
		uint8_t memory[9];

	public:
		uint8_t getSize(){ return sizeof(memory); };

		uint8_t &operator [](uint8_t i);
	};

private:
	OWBus &bus;
	OWBus::OWAddress &addr;
	ScrachPad spad;

public:
	OWDevice( OWBus &abus, OWBus::OWAddress &aa ) : bus(abus), addr(aa){};

	OWBus::OWAddress &getAddress(){ return addr; }
	bool ifConnected();
	ScrachPad &getSratchPad(){ return spad; }
};

#endif
