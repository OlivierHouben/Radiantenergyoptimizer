/*
 Name:		Essaipatern.ino
 Created:	06/12/2017 14:24:05
 Author:	Houbix
*/

unsigned long previousMicros = 0;

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
const long interval2 = 300;

unsigned long currentMicros = 0;
int numeroEssai = 0;


double period = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(10, OUTPUT); 
	digitalWrite(10, HIGH);// set outPin pin as output
}

// the loop function runs over and over again until power down or reset
void loop() {


	for(double frequency=10000;frequency<=50000;frequency=frequency+5000)
	{
		period=1000000/frequency;
		for (double i=1;i<=10;i++)
		{
			digitalWrite(10, HIGH);
			currentMicros = micros();
			previousMicros = micros();
			while (currentMicros - previousMicros <i*0.1*period)
			{
				currentMicros = micros();



			}
			previousMicros = currentMicros;

			double on = i*0.1*period;
			digitalWrite(10, LOW);
			currentMicros = micros();
			previousMicros = micros();
			while (currentMicros - previousMicros <(period-(i*0.1*period)))
			{
				currentMicros = micros();



			}
			previousMicros = currentMicros;
		}
		
	}
	
	/*currentMillis = millis();
	previousMillis = currentMillis;
	while (currentMillis - previousMillis < interval)
	{
		currentMillis = millis();



	}
	previousMillis = currentMillis;*/
	
}
	
