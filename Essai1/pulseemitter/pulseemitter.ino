/*
 Name:		essaiforum.ino
 Created:	08/12/2017 10:28:40
 Author:	olivi
*/

// the setup function runs once when you press reset or power the board
// 2000000 because 16/8
#define freqcpu 2000000
#define nombrevaleurtableau 1000

// declaration valeur 
volatile boolean envoipulse;
volatile int Tableautensioncondo[nombrevaleurtableau];
volatile int pointeurtableau = 0;
volatile boolean ADCfinish;

int pulsesent;

long frequence = 10000;

int tableauOCRBTon[10][10];

int tableaufrequenceOCRA[10];

int numerofrequence = 0;

unsigned long tstartADCgeneral;
unsigned long tactuADCgeneral;
volatile unsigned long tableauTstartADC[nombrevaleurtableau];

// car sur le timer 2 on a prescale 8



void setup() {
	Serial.begin(2000000);
	Serial.print("heyo");
	timer3init();// interupt toute les 5sec
	timer1init();// ADC
	//timer2init(); // envoie 10 pulse
	
	pinMode(9, OUTPUT);
	pinMode(A0, INPUT);
	for (int i = 0; i < 10; i++)
	{

		tableaufrequenceOCRA[i] = freqcpu / (frequence);
		//Serial.println(tableaufrequenceOCRA[i]);
		for (int j = 0; j < 10; j++)
		{
			tableauOCRBTon[i][j] = 0.1*(j + 1)*tableaufrequenceOCRA[i];

			//Serial.print(tableauOCRBTon[i][j]);
			//Serial.print("  ");
		}
		//Serial.println();


		frequence = frequence + 5000;
	}

}

// the loop function runs over and over again until power down or reset
void loop() {


	
	if (envoipulse) // signal des 4 seconde on envoie 
	{
		
		pulsesent = 0; // on reinitialise pour bien envoyer 10 pulses lors de la prochaine itération

		timer2enable(); // on autorise le timer a commencer a compter pour generer les 10 cycles



		TIMSK1 |= (1 << OCIE1A);// on autorise le debut de l'echantillonage ADC
		tstartADCgeneral = millis();
		while (pulsesent < 10) // on boucle tant que on a pas envoyer les 10 pulses
		{


			if (TIFR2 & (1 << TOV2)) // on poll le flag pour voir si il y'a eu l'envoie de un pulse
			{
				pulsesent++;  // incremente pour avoir le prochain dutycyle
				OCR2B = tableauOCRBTon[numerofrequence][pulsesent]; // fetch le nouveau comparateur dans le OCR2B
				//if (OCR2B == OCR2A) // petit tweak pour empecher un bug , on reste a 99% max de duty cycle
					//OCR2B--;

				TIFR2 |= (1 << TOV2); // reclear le flag


				
			}

			if (ADCfinish = true)// on imprime sur le serial
			{
				Serial.println(tableauTstartADC[pointeurtableau]);
				Serial.println(Tableautensioncondo[pointeurtableau]);

				ADCfinish = false;
			}


		}
		TCCR2A = 0; // reset le timer2
		TCCR2B = 0;

		tactuADCgeneral = millis();
		while (tactuADCgeneral - tstartADCgeneral < 1000)
		{

			if (ADCfinish = true)
			{
				Serial.println(tableauTstartADC[pointeurtableau]);
				Serial.println(Tableautensioncondo[pointeurtableau]);

				ADCfinish = false;
			}
			tactuADCgeneral = millis();

		}
		TIMSK1 |= (0 << OCIE1A);

		

		numerofrequence++;
		if (numerofrequence > 9) // numero frequence est le pointeur du tableau des frequences
		{
			numerofrequence = 0;
		}
		envoipulse = false;

	}






}


ISR(TIMER3_OVF_vect)// toute les 4.16 sec
{

	envoipulse = true;
	
	TCNT3 = 50000;

}


ISR(TIMER1_COMPA_vect) // 5 khz toute les 200us
{
	pointeurtableau++;
	if (pointeurtableau > nombrevaleurtableau)
		pointeurtableau = 0;

	tableauTstartADC[pointeurtableau] = micros();
	Tableautensioncondo[pointeurtableau] = analogRead(A0);

	ADCfinish = true;
	
}

void timer3init() {
	TCCR3B = 0;                //stop the timer
	TCCR3A = 0;
	TIFR3 = 0xFF;              //ensure all interrupt flags are cleared
	OCR3A = 65000;
	OCR3B = 65000;//timer runs at 16MHz / 65000 / 1024 (prescaler) = 4.16s period
	cli();
	TCNT3 = 0;                           //clear the timer
	TIMSK3 |= (1 << TOIE3);                //enable timer interrupts
	sei();
	TCCR3A |= (0 << COM3A1) | (0 << COM3A0) | (0 << COM3B1) | (0 << COM3B0) | (0 << WGM31) | (0 << WGM30);
	TCCR3B |= (0 << WGM33) | (0 << WGM32) | (1 << CS32) | (1 << CS30); // 1024 // normal mode
}


void timer2enable() {
	// init timer 2

	TIMSK2 = 0;
	OCR2A = tableaufrequenceOCRA[numerofrequence];
	OCR2B = tableauOCRBTon[numerofrequence][pulsesent];
	cli();
	TCNT2 = 0;                         //clear the timer										
	sei(); //enable timer interrupts

	TCCR2A |= (0 << COM2A1) | (0 << COM2A0) | (1 << COM2B1) | (0 << COM2B0) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22) | (0 << CS22) | (1 << CS21) | (0 << CS20); //8 // fast PWM
}

void timer1init() {
	TCCR1B = 0;                //stop the timer
	TCCR1A = 0;
	TIFR1 = 0xFF;              //ensure all interrupt flags are cleared
	OCR1A = 3200;
	//timer runs at 16MHz / 3200 / 1 (prescaler) = 200 us period
	cli();
	TCNT1 = 0;                           //clear the timer
	//enable timer interrupts
	sei();
	TCCR1A |= (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
	TCCR1B |= (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS10); // 1// ctc
}



