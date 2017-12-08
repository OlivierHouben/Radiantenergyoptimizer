// slightly more accurate usec blink

#include "dwt_timer.h"
#define PIN PB4
static const dwt_timer usec_timer;

void setup() {
	pinMode(10, OUTPUT);
	usec_timer.init();
}

void loop() {
	uint32_t s0, e0;
	s0 = usec_timer.get();
	digitalWrite(10, 1);
	e0 = usec_timer.get();
	const uint32_t overhead = e0 - s0;
	digitalWrite(10, 0);
	const uint32_t overhead1 = (e0 - s0) + 21; /* the 21 hmm .. just kept tweaking until it was 10 usec cycle*/

	while (1) {
		digitalWrite(10, LOW);
		usec_timer.delay_cycles(usec2cycles(100) - (overhead));
		digitalWrite(10, HIGH);
		usec_timer.delay_cycles(usec2cycles(900) - (overhead1));
	}
}