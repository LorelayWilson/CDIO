#include <Arduino.h>
#include <SensorLuz.h>
Adafruit_ADS1115 ads1115(0x48); 

void medirLuz(int VTBpin, int saturacion, int oscuro)
{
  	
    int16_t v = ads1115.readADC_SingleEnded(VTBpin); //Lectura del pin VTBpin o AO.
    int16_t  voltaje = (4096 / (pow(2, 15) - 1)) *  v;

    if(voltaje <= oscuro) Serial.println("Es de noche o el sensor esta tapado."); 
    if(voltaje >= saturacion) Serial.println("Completamente soleado.");
    if(voltaje > oscuro && voltaje < saturacion) Serial.println("Con sombra o nuves.");
     
	delay(2000);

}
