/* This example demonstrates how to read DS2406's PIOs
 *
 * Connect a switch between PIO.A and Vcc through a >5k resistor
 *
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS2406.h>

#define ONE_WIRE_BUS 5			// Where 1W bus is connected to (ESP-201)
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

#define PROBE_ADDR 0x121a08d00000002a	// Address of the probe to manage

void setup() {
    Serial.begin(115200);
    delay(100);

	Serial.println("Probe's information :\n"
				   "---------------------" );
	DS2406 probe(bus, PROBE_ADDR);
	Serial.println( probe.hasPIOB() ? "Both PIO.A & PIO.B" : "only PIO.A" );
	Serial.println( probe.isParasitePowered() ? "Parasite" : "External" );
}

void loop() {
	DS2406 probe(bus, PROBE_ADDR);

	for(;;){
		Serial.print("PIOs are :");
		Serial.print(probe.readPIOs(), HEX);
		Serial.print(probe.getPIOA() ? " PIO.A : on," : " PIO.A : off,");
		Serial.println(probe.hasPIOB() ? (probe.getPIOB() ? " PIO.B : on" : " PIO.B : off") : " PIO.B : Absent");
		delay(5e2);
	}
}
