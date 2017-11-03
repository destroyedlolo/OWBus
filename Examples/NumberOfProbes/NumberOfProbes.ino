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
	Serial.print("Number of probes on the bus :");
	Serial.println(bus.getDeviceCount());

	Serial.println("\nIndividual address :");
	OWBus::Address addr;
	bus.search_reset();
	while( bus.search_next( addr ) ){
		Serial.println( addr.toString().c_str() );
		OWDevice probe( bus, addr );
	}

	delay(30e3);	// Sleep for 30 seconds
}
