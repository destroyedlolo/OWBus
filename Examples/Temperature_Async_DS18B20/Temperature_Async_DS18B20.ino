/* This example show how to mesure asynchronously temperature 
 * using a DS18B20 probe 
 * 
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS18B20.h>

#define ONE_WIRE_BUS D1					// Where O1 bus is connected to
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
 	Serial.begin(115200);
	delay(100);
}

void loop() {
	DS18B20 probe(bus, 0x2882b25e09000015 );
	bool parasite = probe.isParasitePowered();
	Serial.println( parasite ? "Parasite":"External" );

	probe.launchTemperatureAquisition( parasite );	// Launch conversion
	delay( probe.getConversionDelay() );			// Wait as per precision
	Serial.println( probe.readLastTemperature() );

	delay(10e3);
}
