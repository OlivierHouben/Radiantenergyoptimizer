// slightly more accurate usec blink

#include "dwt_timer.h"

static const dwt_timer usec_timer;

void setup() {
	pinMode(10, OUTPUT);
	
}

void loop() {
	

	while (1) {
		digitalWrite(10,HIGH);
		while (1) {

		}
		digitalWrite(10,LOW);
		usec_timer.delay_cycles(1480);
	}
}