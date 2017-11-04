/* This example shows how to play with DS28EA00's PIOs
 *	- PIO.A is in output
 *	- PIO.B is in input
 *
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS28EA00.h>

#define ONE_WIRE_BUS D1			// Where O1 bus is connected to
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
	Serial.begin(115200);
	delay(100);
}

void loop() {
	DS28EA00 probe(bus, 0x4292524700000012 );

	if( probe.writePIOs( DS28EA00::PIOmask::PIOAmask ) )	// Drop PIO.A low
		Serial.println("PIO.A low");
	else
		Serial.println("writePIOs failure");
	delay(1e3);

	if( probe.writePIOs( DS28EA00::PIOmask::PIONONEmask ) )	// Drop PIO.A low
		Serial.println("PIO.A High");
	else
		Serial.println("writePIOs failure");	
	delay(10e3);

}
