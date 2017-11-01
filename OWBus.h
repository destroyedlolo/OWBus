/* One wire bus handling
 *
 * This library is cover by CC BY-NC-SA licence.
 * https://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 * This library 
 * 	- needs OneWire library (http://www.pjrc.com/teensy/td_libs_OneWire.html)
 * 	- is trying to stay compatible with Arduino-Temperature-Control-Library
 * 		(https://github.com/milesburton/Arduino-Temperature-Control-Library)
 *
 * 01/11/2017 - L.Faillie
 *
 */
#ifndef OWBus_H
#define OWBus_H 0.0100

#include <OneWire.h>


class OWBus {
	OneWire *ow;

public:
	OWBus(OneWire *aow): ow(aow){
	}
	void begin( void ){};

		/* Addresses */
	class OWAddress {
		uint8_t addr[8];
	public:
		uint8_t *operator *(){
			return addr;
		}

		String toString();
	};

		/* Search functions */
	void search_reset();
	bool search_next(OWAddress &);
	uint8_t getDeviceCount(void);
};

#endif
