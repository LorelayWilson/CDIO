/*
 *  Humedad.cpp - Libreria para humedad
 */

#include "Arduino.h"
#include "Humedad.h"

Humedad::Humedad(int airValue, int waterValue, int adc){

    this.airValue=airValue;
    this.waterValue=waterValue;
    adcH=adc;

}

//Función de humedad para calcular la humedad
int16_t Humedad::getHumedad() {

  //Guardamos el valor que mide el sensor de humedad que se comunica con el pin A0
  int16_t adc = ads1115.readADC_SingleEnded(adcH);
  //Guardamos en humedad el calculo del porcentaje
  int16_t humedad = 100 * airValue / (airValue - waterValue) - adc * 100 / (airValue - waterValue);

  return humedad;
}

//Función para mostrar por pantalla
void Humedad::mensajeError(int16_t humedad){
  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if (adc0 < (waterValue - H) or adc0 > (airValue + H)) {
    Serial.print("Error lectura humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  } else {
    Serial.print("La humedad es: ");
    Serial.print(humedad);
    Serial.println("%");
  }
  
}
