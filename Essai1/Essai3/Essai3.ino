#include <PWM.h>
#include "TimerThree.h"

// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.

const int led = LED_BUILTIN;  // the pin with a LED
volatile int tableau[3000];
volatile int pointeur=0;
volatile boolean ADCdone;

void setup(void)
{
	Serial.begin(9600);
	Serial.println("coucou");
	pinMode(led, OUTPUT);
	Timer3.initialize(100000);
	Timer3.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds
	
	pinMode(A0, INPUT);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED(void)
{
	//if (ledState == LOW) {
	//	ledState = HIGH;
	//	blinkCount = blinkCount + 1;  // increase when LED turns on
	//}
	//else {
	//	ledState = LOW;
	//}
	//digitalWrite(led, ledState);
	//Serial.println(millis());
	if (pointeur >= 3000)
		pointeur = 0;

	if (pointeur!=0)
	pointeur++;

	tableau[pointeur] = analogRead(A0);
		
		ADCdone = true;

}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
	unsigned long blinkCopy;  // holds a copy of the blinkCount

							  // to read a variable which the interrupt code writes, we
							  // must temporarily disable interrupts, to be sure it will
							  // not change while we are reading.  To minimize the time
							  // with interrupts off, just quickly make a copy, and then
							  // use the copy while allowing the interrupt to keep working.
	noInterrupts();
	blinkCopy = blinkCount;
	interrupts();
	if (ADCdone)
	{
		Serial.println(tableau[pointeur]);
		ADCdone = false;
	}
	//Serial.println(analogRead(A0));
	

	//Serial.print("blinkCount = ");
	//Serial.println(blinkCopy);
	
	
}