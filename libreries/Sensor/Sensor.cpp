/*
 *  Sensor.cpp - Libreria para los sensores
 */

#include "Sensor.h"

// -------------- FUNCIONES PARA MOSTRAR POR PANTALLA --------------

//Función de humedad para calcular y mostrar por pantalla el porcentaje de humedad
void medirHumedad(int airValue, int waterValue, int16_t adc) {

  int H = 600; //constante que usaremos como margen de error para humedad
  //Creamos las variables donde guardaremos el porcentaje de humedad y la lectura del sensor
  
  //Guardamos en humedad el calculo del porcentaje
  int16_t humedad = 100 * airValue / (airValue - waterValue) - adc * 100 / (airValue - waterValue);

  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if (adc < (waterValue - H) or adc > (airValue + H)) {
    Serial.print("Error lectura humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  } else {
    Serial.print("La humedad es: ");
    Serial.print(humedad);
    Serial.println("%");
  }
}


//Descripción de la función: muestra por pantalla el porcentaje de salinidad
void medirSalinidad(int noSalineValue, int maxSalineValue, int16_t adc) {

  int S = 500; //constante que usaremos como margen de error para salinidad

  //Creamos variables donde guardaremos el porcentaje de salinidad y la lectura del sensor
  int16_t salinidad;

  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  salinidad = ((adc - noSalineValue) * 100) / (maxSalineValue - noSalineValue);

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


void medirTemperatura(int ordenada_calibrado, int pendiente_calibrado, int16_t adc) {

  //Creamos las variables donde guardaremos el porcentaje d y la lectura del sensor
  int16_t temperatura, voltaje;

  //Guardamos el valor que mide el sensor de temperatura que se comunica con el pin A2

  voltaje = (4096 / (pow(2, 15) - 1)) * adc;

  //Guardamos en humedad el calculo del porcentaje
  temperatura = (voltaje - ordenada_calibrado) / pendiente_calibrado;
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}