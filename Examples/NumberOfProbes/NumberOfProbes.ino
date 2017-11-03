/* This example shows how to enumerate probes
 *  using OWBus
 *  
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/OWDevice.h>

#define ONE_WIRE_BUS D1					// Where O1 bus is connected to
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
	Serial.begin(115200);
	delay(100);
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
		if(!addr.isValide( &oneWire))
			Serial.println("Invalide address");
		else {
			OWDevice probe( bus, addr );
			Serial.println( probe.getFamilly() );
		}
	}

	delay(30e3);	// Sleep for 30 seconds
}
