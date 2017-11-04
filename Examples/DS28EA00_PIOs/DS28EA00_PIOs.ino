/* This example shows how to play with DS28EA00's PIOs
 *	- PIO.A is in output
 *	- PIO.B is in input
 *
 *	If you connect PIO.A with PIO.B and them to VDD trough a resistor,
 *	you will see that PIO.B takes the value of PIO.A
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
//	DS28EA00 probe(bus, 0x4292524700000012 );
	DS28EA00 probe(bus, 0x42886847000000bf );

	if( probe.writePIOs( DS28EA00::PIOmask::PIOAmask ) )	// Drop PIO.A low
		Serial.println("\nPIO.A low");
	else
		Serial.println("\nwritePIOs failure");
	delay(1e3);

	uint8_t val = probe.readPIOs();
	if( !val || !probe.arePIOsValide())
		Serial.println("readPIOs() failure");
	else {
		Serial.print( probe.PIOB() ? "PIO.B is high > " : "PIO.B is low > ");
		Serial.print( val, BIN);
		Serial.print( " : 0x");
		Serial.println( val, HEX);
	}

	if( probe.writePIOs( DS28EA00::PIOmask::PIONONEmask ) )	// Drop PIO.A low
		Serial.println("\nPIO.A High");
	else
		Serial.println("\nwritePIOs failure");	
	delay(1e3);

	val = probe.readPIOs();
	if( !val || !probe.arePIOsValide())
		Serial.println("readPIOs() failure");
	else {
		Serial.print( probe.PIOB() ? "PIO.B is high > " : "PIO.B is low > ");
		Serial.print( val, BIN);
		Serial.print( " : 0x");
		Serial.println( val, HEX);
	}
}
