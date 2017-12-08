/*
Name:		Essaipatern.ino
Created:	06/12/2017 14:24:05
Author:	Houbix
*/

#include <eRCaGuy_Timer2_Counter.h>
#include <digitalWriteFast.h>


unsigned long timeelapsed;

int period = 0;

float  tempsON[11];
float tempsOFF[11];
// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(10, OUTPUT);
	pinMode(13, OUTPUT);
	digitalWrite(10, HIGH);// set outPin pin as output
	Serial.begin(115200);
	timer2.setup();
	period = 1000000 /12000; //40 khz frequency

	for (int i = 1; i <= 10; i++)
	{
		tempsON[i] = 0.1*i*period;
		tempsOFF[i] = period - 0.1*i*period;

	}

}

// the loop function runs over and over again until power down or reset
void loop() {
		
		for (int i = 1; i <= 10; i++)
		{	   
									 
			
			 unsigned long previousT = timer2.get_count();
			 digitalWriteFast(10, HIGH);
				unsigned long currenT = previousT;
				
			  timeelapsed = 0;

			 
			while (((float)timeelapsed)<(tempsON[i]))
			{

				 currenT = timer2.get_count();	
				 timeelapsed = (int)(currenT-previousT)/2;
				 
			}
				 /* Serial.print("temps écoulé ");
			Serial.println(timeelapsed);
			Serial.print("TON ");
			Serial.println(tempsON[i]);*/
				
						
			unsigned long previousT2 = currenT;
			digitalWriteFast(10, LOW);
			unsigned long currenT2 = previousT2;
			timeelapsed = 0;
			
			while ((float)timeelapsed<tempsOFF[i])
			{
				currenT2= timer2.get_count();
				timeelapsed = (int)(currenT2 - previousT2)/2;
			}
		
			/*
			Serial.print("temps écoulé ");
			Serial.println(timeelapsed);
			Serial.print("TOOFF ");
			Serial.println(tempsOFF[i]);
			*/
				

			
			
		
		
			
		}
		delay(100);
		//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	

	

}

