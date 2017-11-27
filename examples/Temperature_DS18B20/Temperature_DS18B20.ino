/* This example show how to measure temperature in the simplest way
 * using a DS18B20 probe 
 * 
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS18B20.h>

#define ONE_WIRE_BUS 5					// Where 1W bus is connected to
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
 	Serial.begin(115200);
	delay(100);
}

void loop() {
	DS18B20 probe(bus, 0x2882b25e09000015 );

		/* It's optional, you can safely use getTemperature() without
		 * argument. In this cas, parasite-power mode is used which is
		 * in any case the safer way even on externally powered probe.
		 */
	bool parasite = probe.isParasitePowered();
	Serial.println( parasite ? "Parasite":"External" );

	Serial.println( probe.getTemperature( parasite ) );

	delay(10e3);
}
