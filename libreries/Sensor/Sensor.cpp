/*
 *  Sensor.cpp - Libreria para los sensores
 */

#include "Sensor.h"

// --------------- CONSTRUCTORES -------------

Sensor::Sensor(){

}

Sensor::Sensor(int pin, double min_valor, double max_valor, Adafruit_ADS1115 ads){
  pinADC = pin;
  valor_min = min_valor;
  valor_max = max_valor;
  ads1115 = ads;
  pin = 0;
  lectura = 0;
}

Sensor::Sensor(int pin, double min_valor, double max_valor, int pinC, Adafruit_ADS1115 ads){
  pinADC = pin;
  min_valor = min_valor;
  max_valor = max_valor;
  pin = pinC;
  ads1115 = ads;
  lectura = 0;
}


// -------------- FUNCIONES PARA MOSTRAR POR PANTALLA --------------

//Función para humedad
void Sensor::lecturaHumedad(){

  lectura = ads1115.readADC_SingleEnded(pinADC);
  int16_t humedad = 100 * valor_min / (valor_min  - valor_max) - lectura * 100 / (valor_min - valor_max);
  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if (lectura < (valor_max - 500) or lectura > (valor_min + 500)) {
    Serial.print("Error lectura humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  } else {
    Serial.print("La humedad es: ");
    Serial.print(humedad);
    Serial.println("%");
  }
  
}

//Función para salinidad

void Sensor::lecturaSalinidad(){

  digitalWrite(pin, HIGH); 
  delay(100); 
  lectura = ads1115.readADC_SingleEnded(pinADC);
  digitalWrite( pin_sal, LOW ); 
  delay(10); 
  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  int16_t salinidad = ((lectura - valor_min)* 100)/(valor_max - valor_min);

 //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if (lectura < (valor_min - 500) or lectura > (valor_max + 500)) {
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

//Funcion para temperatura

void Sensor::lecturaTemperatura(){

  lectura = ads1115.readADC_SingleEnded(pinADC);

  int16_t voltaje = (4096 / (pow(2, 15) - 1)) * lectura;
  Serial.println(lectura);
  Serial.println(voltaje);
  //Guardamos en humedad el calculo del porcentaje
  double temperatura = (voltaje - valor_max) / valor_min;
  //Bucle que usamos para mandar mensaje
  Serial.print("Temperatura es: ");
  Serial.println(temperatura);
}