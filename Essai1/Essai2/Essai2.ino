#define ledPin 13

void setup()
{
	pinMode(ledPin, OUTPUT);

	// initialize timer1 
	noInterrupts();           // disable all interrupts
	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3 = 0;

	OCR3A = 62500;            // compare match register 16MHz/256/2Hz
	TCCR3B |= (1 << WGM32);   // CTC mode
	TCCR3B |= (1 << CS32);
	TCCR3B |= (1 << CS30); // 256 prescaler 
	TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
	interrupts();             // enable all interrupts
}

ISR(TIMER3_COMPA_vect)          // timer compare interrupt service routine
{
	digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin
}

void loop()
{
	// your program here...
}
