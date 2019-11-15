/*
 *  Humedad.cpp - Libreria para humedad
 */

#include "Arduino.h"
#include "Salinidad.h"

Salinidad::Salinidad(int noSalineValue, int maxSalineValue, int adc, int pin){

    this.noSalineValue=noSalineValue;
    this.maxSalineValue=maxSalineValue;
    adcS=adc;
    pinMode(pin, OUTPUT); //Configuramos el pin como salida
    this.pin = pin;
}

//Función de humedad para calcular la humedad
int16_t Salinidad::leerADC() {

  digitalWrite(pin, HIGH);
  delay(100);

  //Guardamos el valor que mide el sensor de salinidad que se comunica con el pin A1
  int16_t adc = ads1115.readADC_SingleEnded(adcS);
  digitalWrite(pin, LOW);
  delay(10);

  return adc;
}

//Función de humedad para calcular la humedad
int16_t Salinidad::getSalinidad(int16_t adc) {

  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  int16_t salinidad = ((adc - noSalineValue) * 100) / (maxSalineValue - noSalineValue);

  return salinidad;
}

//Función para mostrar por pantalla
void Salinidad::mensajeError(int16_t salinidad, int16_t adc){
 //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if (adc < (noSalineValue - S) or adc > (maxSalineValue + S)) {
    Serial.print("Error de lectura salinidad: ");
    Serial.print("La salinidad es: ");
    Serial.print(salinidad);
    Serial.println(" %");

  } else {
    Serial.print("La salinidad es: ");
    Serial.print(salinidad);
    Serial.println(" %");
  }
  
}