#include <Arduino.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads1115(0x48); 
   const int VTBpin = 0;

void setup() 
{
   Serial.begin(9600); 
   ads1115.begin();
   ads1115.setGain(GAIN_ONE);
}
 
void loop()
{
  int16_t v = ads1115.readADC_SingleEnded(VTBpin); //Lectura del pin VTBpin o AO.
  int16_t  voltaje = (4096 / (pow(2, 15) - 1)) *  v;

 if(voltaje <= 25) Serial.println("Es de noche o el sensor está tapado."); 
 if(voltaje >= 1814) Serial.println("Completamente soleado.");
 if(voltaje > 25 && voltaje < 1814) Serial.println("Con sombra.");
     
   delay(2000);
}
