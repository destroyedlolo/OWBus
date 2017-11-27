/* This example demonstrates how to blink a LED on DS2406's PIO.A
 *
 * Connect the cathode of the LED to DS2406 pin 3 (PIO.A), 
 * then LED's anode to a 330ohms or more resistor and then to +3,3v
 *
 * Note : PIO.B is too weak to drive LEDs
 *	
 *  This little piece of code is put in Public Domain.
 */

#include <OWBus.h>
#include <OWBus/DS2406.h>

#define ONE_WIRE_BUS 5			// Where 1W bus is connected to (ESP-201)
OneWire oneWire(ONE_WIRE_BUS);	// Initialize oneWire library
OWBus bus(&oneWire);

void setup() {
    Serial.begin(115200);
    delay(100);
}

void loop() {
	DS2406 probe(bus, 0x121a08d00000002a);

	for(;;){
		probe.setPIOA( true );
		Serial.println("On");
		delay(5e2);
		probe.setPIOA( false);
		Serial.println("Off");
		delay(5e2);
	}
}
