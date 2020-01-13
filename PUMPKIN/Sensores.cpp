/*
 *  Sensores.cpp - Libreria para los sensores
 */

#include "Sensores.h"


// ======CONSTRUCTOR======

Sensores::Sensores(){
}


Sensores::Sensores(int _pin, double _max_valor, double _min_valor, Adafruit_ADS1115 _ads){
  pin = _pin;
  max_valor = _max_valor;
  min_valor = _min_valor;
  ads1115 = _ads;
  power_pin = 0;
  lectura = 0;
}

Sensores::Sensores(int _pin, int _power_pin, double _max_valor, double _min_valor, Adafruit_ADS1115 _ads){
  pin = _pin;
  max_valor = _max_valor;
  min_valor = _min_valor;
  power_pin = _power_pin;
  ads1115 = _ads;
  lectura = 0;
}

// -------------- METODOS UTILIDAD --------------

void Sensores::leerSensorInterruptor() {

 digitalWrite(power_pin, HIGH); // Activamos el sensor // @suppress("Invalid arguments")
 delay(100); // Esperamos para la lectura
 lectura = ads1115.readADC_SingleEnded(pin); // @suppress("Invalid arguments")
 digitalWrite(power_pin, LOW); // Desactivamos el sensor // @suppress("Invalid arguments")
}

void Sensores::leerSensor() {
  lectura = ads1115.readADC_SingleEnded(pin); // @suppress("Invalid arguments")
}

void Sensores::obtenerVoltaje(){
  lectura = (4096 / (pow(2, 15) - 1)) * lectura;
}

// -------------- FUNCIONES PARA MOSTRAR POR PANTALLA --------------

/* Función para devolver por pantalla la lectura de la humedad
*/
void Sensores::medirHumedad() {
  //Min Valor = Air Value  ||  Max Valor = Water Value

  leerSensor();
  int16_t humedad = 100 * min_valor / (min_valor - max_valor) - lectura*100 / (min_valor - max_valor);

   //Realizamos unos límites para que no pueda superar el 100% o ser más pequeño que el 0%
  if(lectura < max_valor){
      Serial.print("La humedad es: ");
      Serial.println("100 %");
  } else if (min_valor > lectura){
      Serial.print("La humedad es: ");
      Serial.println("0 %");
  } else{
      Serial.print("La humedad es: ");
      Serial.print(humedad);
      Serial.println(" %");
  }

}

/* Función para devolver por pantalla la lectura de la salinidad
*/
void Sensores::medirSalinidad() {

  // Max Valor = Max_Salinidad || Min Valor = Min_Salinidad

  leerSensorInterruptor();
  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  int16_t salinidad = ((lectura- min_valor) * 100) / (max_valor - min_valor);

  //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
    if(lectura < min_valor){
      Serial.print("La salinidad es: ");
      Serial.println("0 %");
  } else if (lectura > max_valor){
      Serial.print("La salinidad es: ");
      Serial.println("100 %");
  } else{
      Serial.print("La salinidad es: ");
      Serial.print(salinidad);
      Serial.println(" %");
  }
}

/* Función para devolver por pantalla la lectura de la temperatura
*/
void Sensores::medirTemperatura() {

  // Max Valor = ordenada (B) || Min Valor = Pendiente (M)

  leerSensor();

  //Obtenemos el valor que mide el sensor de temperatura que se comunica con el pin A2

  obtenerVoltaje();

  //Guardamos en humedad el calculo del porcentaje
  int16_t  temperatura = (lectura - max_valor) / min_valor;
  
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}

/* Función para devolver por pantalla la lectura de la iluminacion
*/
void Sensores::medirLuz(){
  //Max Valor = Max Luz || Min Valor = Min Luz

    
  //Obtenermos el valor que mide el sensor de iluminacion que se comunica con el pin A3
  obtenerVoltaje();

  //Mostrar por pantalla segun los valores de calibración
  if(lectura <= min_valor) Serial.println("Es de noche o el sensor esta tapado."); 
  if(lectura >= max_valor) Serial.println("Completamente soleado.");
  if(lectura > min_valor && lectura < ((max_valor-min_valor)/2)) Serial.println("Bastante nublado.");
  if(lectura > ((max_valor-min_valor)/2) && lectura < max_valor) Serial.println("Poco nublado.");
   
}
