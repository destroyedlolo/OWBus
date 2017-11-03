/* Handle scratchpad
 *
 * 03/11/2017 - L.Faillie - First version
 */
#ifndef OWSCRATCHPAD_H
#define OWSCRATCHPAD_H 0.0101

#include <OWBus/OWDevice.h>

class OWScrachpad {
	uint8_t fake;
	OWDevice *device;

protected:
	size_t size;
	uint8_t *memory;

public:
	OWScrachpad(OWDevice *p, size_t asz) : device(p), size(asz){
		memory = new uint8_t[asz];
	};
	bool read();

	uint8_t getSize(){ return size; };
	uint8_t &operator [](uint8_t i);

};


#endif
