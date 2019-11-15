/*
 *  Temperatura.cpp - Libreria para Temperatura
 */

#include "Arduino.h"
#include "Temperatura.h"

Temperatura::Temperatura(int ordenada, int m, int adc){

    this.ordenada=airValue;
    this.m=m;
    adcT=adc;
}

//Función de humedad para calcular la humedad
int16_t Temperatura::getTemperatura() {

  //Guardamos el valor que mide el sensor de temperatura que se comunica con el pin A2
  int16_t adc2 = ads1115.readADC_SingleEnded(adcT);

  int16_t voltaje = (4096 / (pow(2, 15) - 1)) * adc2;

  //Guardamos en humedad el calculo del porcentaje
  int16_t temperatura = (voltaje - ordenada) / m;

  return temperatura;
}

//Función para mostrar por pantalla
void Temperatura::mensaje(int16_t temperatura){
  //Bucle que usamos para mandar mensaje
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}
