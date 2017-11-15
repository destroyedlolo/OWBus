/* This example shows how to enumerate probes
 *  using OWBus
 *  
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/OWDevice.h>

#if 0	/* Test bits ordering */
#define IMPLEMENT_BITFIELD_TEST
#include <OWBus/DS2406.h>
#endif

#define ONE_WIRE_BUS 5			// Where 1-wire bus is connected to (GPIO-5)
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
	Serial.begin(115200);
	delay(100);

	Serial.print("Flash's size : ");
	Serial.println(ESP.getFlashChipRealSize());
}

void loop() {
	Serial.print("\nNumber of probes on the bus :");
	Serial.println(bus.getDeviceCount());

	Serial.println("Individual address :");
	OWBus::Address addr;
	bus.search_reset();
	while( bus.search_next( addr ) ){
		Serial.print( addr.toString().c_str() );
		Serial.print(" : ");
		if(!addr.isValid( &oneWire))
			Serial.println("Invalid address");
		else {
			OWDevice probe( bus, addr );
			Serial.println( probe.getFamilly() );

#			ifdef IMPLEMENT_BITFIELD_TEST
			if( addr.getFamillyCode() == DS2406::FAMILLY_CODE ){
				DS2406 tst( bus, addr );
				Serial.println(tst.checkArchitecture() ? "Arch ok":"Bad bits ordering");
			}
#			endif
		}
	}

	delay(30e3);	// Sleep for 30 seconds
}
