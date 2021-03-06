/* Handle scratchpad
 *
 * 03/11/2017 - L.Faillie - First version
 */
#ifndef OWSCRATCHPAD_H
#define OWSCRATCHPAD_H 0.0105

#include <OWBus/OWDevice.h>

class OWScratchpad {
	uint8_t fake;
	OWDevice *device;
	size_t size;
	uint8_t *memory;

protected:
	bool virgin;
	uint8_t *getScratchpadMemory(){ return memory; }

public:
	OWScratchpad(OWDevice *p, size_t asz) : device(p), size(asz), virgin(true){
		memory = new uint8_t[asz];
	};
	virtual bool readScratchpad();
	virtual bool writeScratchpad()=0;
	virtual bool isValidScratchpad(){ return true; }	// Derived classes must implement a real check
	virtual void forceInvalidScratchpad(){};

	uint8_t getSize(){ return size; };
	uint8_t &operator [](uint8_t i);
};


#endif
