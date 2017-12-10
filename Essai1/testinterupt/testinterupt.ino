/*
 Name:		testinterupt.ino
 Created:	07/12/2017 18:18:41
 Author:	Houbix
*/

// the setup function runs once when you press reset or power the board
#include <eRCaGuy_Timer2_Counter.h>
#include <digitalWriteFast.h>

volatile int frequence=10000;

unsigned long timeelapsed;

// coucou petite modif de githubweb


void setup() {
	TCCR3B = 0;                //stop the timer
	TCCR3A = 0;
	TIFR3 = 0xFF;              //ensure all interrupt flags are cleared
	OCR3A = 65000; 
	OCR3B = 65000;//timer runs at 16MHz / 65000 / 1024 (prescaler) = 4.16s period
	cli();
	TCNT3 = 0;                           //clear the timer
	TIMSK3 |= (1<<OCIE3B)|(1 << OCIE3A);                //enable timer interrupts
	sei();
	TCCR3A |= (1<< COM3A1)| (0<< COM3A0) | (0 << COM3B1) | (1 << COM3B0) | (0 << WGM31) | (0 << WGM30);
	TCCR3B |= (0 << WGM33) | (1 << WGM32) | (1 << CS32) | (1 << CS30); // 1024 // ctc
	pinMode(LED_BUILTIN, OUTPUT);
	timer1init();
	pinMode(5, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(10, OUTPUT);
	Serial.begin(115200);
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	while (1)
	{
	}
}


void timer1init()
{
	TCCR1B = 0;                //stop the timer
	TCCR1A = 0;
	TCCR1B |=  (1 << WGM12) | (1 << CS12) ; // 256 prescale ctc activated
	OCR1A = 10;
	TIMSK1 |=  (1 << OCIE1A);
}
ISR(TIMER1_COMPA_vect)// 6250 hz interupt for adc sampling beacuse 10 khz is a lot i think
{
	
	// code to read the adc value and stream it
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));


}

ISR(TIMER3_COMPB_vect)// interuption toute les 4.16 sec
{
	
	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	
		
	frequence = frequence + 5000;
	
	if (frequence >= 50000)
		frequence = 10000; // reset la frequence si jamais on depasse 50KHZ

	float period = 1000000 / frequence; 
	float  tempsON[11];
	float tempsOFF[11];

	for (int i = 1; i <= 10; i++)// remplis prealablement le tableau des valeurs TON et TOFF
	{
		tempsON[i] = 0.1*i*period; 
		tempsOFF[i] = period - 0.1*i*period;
	}

	for (int i = 1; i <= 10; i++) // envoi des 10 pulse
	{
		unsigned long previousT = timer2.get_count();// reinitialise les timer pour avoir un bon délai
		digitalWriteFast(10, HIGH);
		unsigned long currenT = previousT;
		timeelapsed = 0;

		while (((float)timeelapsed)<(tempsON[i])) // delai TON
		{
			currenT = timer2.get_count();
			timeelapsed = (int)(currenT - previousT) / 2;
		}	
		
		unsigned long previousT2 = currenT;
		digitalWriteFast(10, LOW);
		unsigned long currenT2 = previousT2;
		timeelapsed = 0;

		while ((float)timeelapsed<tempsOFF[i]) // delai TOFF
		{
			currenT2 = timer2.get_count();
			timeelapsed = (int)(currenT2 - previousT2) / 2;
		}
		
	}
	
}
