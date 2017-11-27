/* This example shows how to 
 *	- enumerate temperature probes
 *	- launch parallel temperature conversion
 *
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS18B20.h>
#include <OWBus/DS28EA00.h>

#define ONE_WIRE_BUS D1			// Where 1W bus is connected to
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
	Serial.begin(115200);
	delay(100);
}

void loop() {
		/* Probes' list */
	DS18B20  probe1(bus, 0x2882b25e09000015 );
	DS28EA00 probe2(bus, 0x42886847000000bf );
	
		/* determine the worst conversion duration */
	unsigned long duration = max(probe1.getConversionDelay(), probe2.getConversionDelay());
	Serial.print("Will wait for :");
	Serial.println(duration);

		/* Determine if we have parasite powered probes 
		 *
		 * Notez-Bien : alternatively, we can broadcast a general 
		 * isParasitePowered() which would be a better solution if we have a
		 * large number of probes, but all probes will respond, including non
		 * temperature ones.
		 * Notez-Bien 2 : in a real project, it would be safer to use parasitic
		 * conversion in any case. It will ensure a strong power supply.
		 */
	bool parasite = max(probe1.isParasitePowered(), probe2.isParasitePowered());
	Serial.println( parasite ? "Parasite mode" : "External mode" );

		/* Broadcast conversions request */
	bus.launchTemperatureAcquisition(parasite);
	delay(duration);	// Waiting for conversions to be done

	Serial.print( (probe1.getAddress().toString() + " (" + probe1.getFamilly() + ") : ").c_str() );
	Serial.println( probe1.readLastTemperature() );
	
	Serial.print( (probe2.getAddress().toString() + " (" + probe2.getFamilly() + ") : ").c_str() );
	Serial.println( probe2.readLastTemperature() );

	delay(30e3);	// Sleep for 30 seconds
}
