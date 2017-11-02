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
		OWDevice *device;

	protected:
		uint8_t memory[9];

	public:
		ScrachPad(OWDevice *p) : device(p) {};
		bool read();

		uint8_t getSize(){ return sizeof(memory); };
		uint8_t &operator [](uint8_t i);

	};

private:
	OWBus &bus;
	OWBus::Address &addr;
	ScrachPad spad;

public:
	OWDevice( OWBus &abus, OWBus::Address &aa ) : bus(abus), addr(aa), spad(this){};

	OWBus &getBus(){ return bus; }
	OWBus::Address &getAddress(){ return addr; }
	bool ifConnected();
	ScrachPad &SratchPad(){ return spad; }
};

#endif
