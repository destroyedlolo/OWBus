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

		/* Addresses */
	class Address {
		uint8_t addr[8];
	public:
		Address(){};
		Address( uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6, uint8_t a7 ){
			addr[0] = a0; addr[1] = a1; addr[2] = a2; addr[3] = a3;
			addr[4] = a4; addr[5] = a5; addr[6] = a6; addr[7] = a7;
		}
		Address( uint8_t a[8] ){ memcpy(addr, a, sizeof(addr)); };
		Address( uint64_t a){
			addr[0] = a >> 56;
			addr[1] = a >> 48;
			addr[2] = a >> 40;
			addr[3] = a >> 32;
			addr[4] = a >> 24;
			addr[5] = a >> 16;
			addr[6] = a >> 8;
			addr[7] = a & 0xff;
		}

		uint8_t *operator *(){
			return addr;
		}

		String toString();

		bool isValide(OneWire *ow){ return(ow->crc8(addr, 7) == addr[7]); }	// Is a valide address ?
		const char *getFamilly();
		uint8_t getFamillyCode(){ return addr[0]; };
	};

		/* Accessors */
	OneWire *getOWTechLayer(){ return ow; }

		/* Search functions */
	void search_reset();
	bool search_next(Address &);
	uint8_t getDeviceCount(void);

		/* Launch temperature conversion on all probe on the bus
		 * (but ds2422)
		 */
	bool launchTemperatureAquisition( bool parasite=true );
};

#endif
