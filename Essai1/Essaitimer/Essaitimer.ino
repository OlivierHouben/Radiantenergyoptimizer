/*
 Name:		Essaitimer.ino
 Created:	07/12/2017 13:47:19
 Author:	Houbix
*/

// the setup function runs once when you press reset or power the board

#include <TimerHelpers.h>

/* ---------------------------------------------------------------
Test sketch
--------------------------------------------------------------- */

const byte SHUTTER = 11;  // this is OC1A (timer 1 output compare A)

void setup() {
	pinMode(SHUTTER, INPUT);
	digitalWrite(SHUTTER, HIGH);

}  // end of setup

ISR(TIMER1_COMPA_vect)
{
	TCCR1A = 0;        // reset timer 1
	TCCR1B = 0;
}  // end of TIMER1_COMPA_vect

void loop() {
	delay(250);   // debugging

	TCCR1A = 0;        // reset timer 1
	TCCR1B = 0;

	digitalWrite(SHUTTER, HIGH);   // ready to activate
	pinMode(SHUTTER, OUTPUT);

	// set up Timer 1
	TCNT1 = 0;         // reset counter
	OCR1A = 999;       // compare A register value (1000 * clock speed)

					   // Mode 4: CTC, top = OCR1A
	Timer1::setMode(4, Timer1::PRESCALE_1, Timer1::CLEAR_A_ON_COMPARE);

	TIFR1 |= bit(OCF1A);    // clear interrupt flag
	TIMSK1 = bit(OCIE1A);   // interrupt on Compare A Match  

}  // end of loop
