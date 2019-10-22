#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x48
const int AirValue = 20200;  // Medimos valor en seco
const int WaterValue = 12650;  // Medimos valor en agua
const int MaxConductivity = 0;
const int MinConductivity = 0;
const int H = 600; //constante que usaremos como margen de error para humedad

void setup() {
  Serial.begin(9600);
  ads1115.begin();
  ads1115.setGain(GAIN_ONE);

}

void loop() {

  int16_t adc0, humedad, salinidad;
  adc0 = ads1115.readADC_SingleEnded(0);
  
  medirHumedad(adc0);
  medirSalinidad(adc0);
  delay (5000);
 
}

void medirHumedad(int16_t adc0){
  
  int16_t humedad;
  humedad = 100*AirValue/(AirValue-WaterValue)-adc0*100/(AirValue-WaterValue);
  
     if(adc0<(WaterValue - H) or adc0>(AirValue + H)) {
      Serial.print("Error lectura humedad: ");
      Serial.print(humedad);
      Serial.print("%   -   ");
      Serial.println(adc0);
    } else{
      Serial.print("La humedad es: ");
      Serial.print(humedad);
      Serial.print("%   -   ");
      Serial.println(adc0);
    }
}

void medirSalinidad(int16_t adc0){

  int16_t salinidad;
  salinidad = 0;

  if(salinidad<0 or salinidad<100){
    Serial.print("Error de lectura salinidad: ");
    Serial.println(salinidad);
    
  } else {
    Serial.print("La salinidad es: ");
    Serial.println(salinidad);
  }
    
}
