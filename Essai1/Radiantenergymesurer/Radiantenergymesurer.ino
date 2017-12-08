
#define SAMPLE_SIZE 10

volatile boolean adcFlag;
volatile boolean timerFlag;
volatile int adcVal;
volatile int arraye[3800];

void setup(void)
{
	delay(1000);
	Serial.begin(115200);
	Serial.println(F("adcStart_us, adcLapse_us, adcValue"));

	//set up the timer
	TCCR1B = 0;                //stop the timer
	TCCR1A = 0;
	TIFR1 = 0xFF;              //ensure all interrupt flags are cleared
	OCR1A = 199;              //timer runs at 16MHz / 200 / 8 (prescaler) = 10kHz
	OCR1B = 199;

	//set up the adc
	ADMUX = _BV(REFS0);                                //use AVcc as reference
	ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE);     //enable ADC, auto trigger, interrupt when conversion complete
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);    //ADC prescaler: divide by 128
	ADCSRB = _BV(ADTS2) | _BV(ADTS0);                  //trigger ADC on Timer/Counter1 Compare Match B
}

void loop(void)
{

	for (int i = 0; i < 1999; i++)
		arraye[i] = 1;





	unsigned long startTimes[SAMPLE_SIZE];    //adc start time (timer interrupt)
	unsigned long endTimes[SAMPLE_SIZE];      //adc complete time (adc interrupt)
	unsigned int adcVals[SAMPLE_SIZE];        //values from the ADC
	unsigned long tStart;                     //timer interrupt time from micros()
	unsigned long tEnd;                       //adc interrupt time from micros()
	byte i;                                   //array index

	cli();
	TCNT1 = 0;                           //clear the timer
	TIMSK1 = _BV(OCIE1B);                //enable timer interrupts
	sei();
	i = 0;
	TCCR1B = _BV(WGM12) | _BV(CS11);     //start the timer, ctc mode, prescaler 8

	do {
		while (!timerFlag);              //wait for timer interrupt which starts adc conversion
		tStart = micros();               //capture the time
		while (!adcFlag);                //wait for ADC conversion to complete
		tEnd = micros();                 //capture the time
		startTimes[i] = tStart;          //save the times
		endTimes[i] = tEnd;
		cli();
		adcFlag = false;                 //reset the ISR flags
		timerFlag = false;
		adcVals[i] = adcVal;             //save the analog reading
		sei();
	} while (++i < SAMPLE_SIZE);

	TIMSK1 = 0;                          //disable timer interrupts
	TCCR1B = 0;                          //stop the timer

										 //print the data
	Serial.println();
	for (byte i = 0; i<SAMPLE_SIZE; ++i) {
		Serial.print(startTimes[i]);
		Serial.print(", ");
		Serial.print(endTimes[i] - startTimes[i]);
		Serial.print(", ");
		Serial.println(adcVals[i]);
	}
	Serial.flush();
	delay(5000);
}

ISR(ADC_vect)
{
	adcFlag = true;
	adcVal = ADC;
}

ISR(TIMER1_COMPB_vect)
{
	timerFlag = true;
}