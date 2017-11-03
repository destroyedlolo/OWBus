/* Handle scratchpad
 *
 * 03/11/2017 - L.Faillie - First version
 */
#ifndef OWSCRATCHPAD_H
#define OWSCRATCHPAD_H 0.0100

#include <OWBus/OWDevice.h>

class OWScrachPad {
	uint8_t fake;
	OWDevice *device;

protected:
	size_t size;
	uint8_t *memory;

public:
	OWScrachPad(OWDevice *p, size_t asz) : device(p), size(asz){
		memory = new uint8_t[asz];
	};
	bool read();

	uint8_t getSize(){ return sizeof(memory); };
	uint8_t &operator [](uint8_t i);

};


#endif
