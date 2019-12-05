/*
 *  Sensor.cpp - Libreria para los sensores
 */

#include "Sensor.h"


// -------------- FUNCIONES PARA MOSTRAR POR PANTALLA --------------

/* Función para devolver por pantalla la lectura de la humedad
*@param airValue es el valor que hemos obtenido al dejar el sensor al aire
*@param waterValue es el valor que hemos obtenido al dejar el sensor introducido completamente en agua
*@param adc es la lectura del ads
*/
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

  	//Realizamos unos límites para que no pueda superar el 100% o ser más pequeño que el 0%
    if(adc > (waterValue - H) and adc < (waterValue)){
	    Serial.print("La humedad es: ");
	    Serial.println("100 %");
	} if ((airValue + H)  > adc and adc > (airValue)){
	    Serial.print("La humedad es: ");
	    Serial.println("0 %");
	} else{
	    Serial.print("La humedad es: ");
	    Serial.print(humedad);
	    Serial.println(" %");
	}
  }
}


/* Función para devolver por pantalla la lectura de la salinidad
*@param noSalineValue es el valor al tener 0 de salinidad
*@param maxSalineValue es el valor al saturar el agua con sal
*@param adc es la lectura del ads
*/
void medirSalinidad(int noSalineValue, int maxSalineValue, int16_t adc) {

  int S = 500; //constante que usaremos como margen de error para salinidad

  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  int16_t salinidad = ((adc - noSalineValue) * 100) / (maxSalineValue - noSalineValue);

  //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if (adc < (noSalineValue - S) or adc > (maxSalineValue + S)) {
    Serial.print("Error de lectura salinidad: ");
    Serial.print("La salinidad es: ");
    Serial.print(salinidad);
    Serial.println(" %");

  } else {

  	if(adc > (noSalineValue - S) and adc < (noSalineValue)){
	    Serial.print("La salinidad es: ");
	    Serial.println("0 %");
	} if ((maxSalineValue + S) > adc   and adc > (maxSalineValue)){
	    Serial.print("La salinidad es: ");
	    Serial.println("100 %");
	} else{
	    Serial.print("La salinidad es: ");
	    Serial.print(salinidad);
	    Serial.println(" %");
	}
  }
}

/* Función para devolver por pantalla la lectura de la temperatura
*@param ordenada_calibrado es la ordenada de la recta de calibracion
*@param pendiente_calibrado es la pendiente de la recta de calibracion
*@param adc es la lectura del ads
*/
void medirTemperatura(int ordenada_calibrado, int pendiente_calibrado, int16_t adc) {

  //Guardamos el valor que mide el sensor de temperatura que se comunica con el pin A2

  int16_t  voltaje = (4096 / (pow(2, 15) - 1)) * adc;

  //Guardamos en humedad el calculo del porcentaje
  int16_t  temperatura = (voltaje - ordenada_calibrado) / pendiente_calibrado;
  
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}

/* Función para devolver por pantalla la lectura de la iluminacion
*@param saturacion es el valor maximo de calibracion cuando hay maxima luz
*@param oscuro es el valor minimo de calibracion cuando está a oscuras
*@param adc es la lectura del ads
*/
void medirLuz(int saturacion, int oscuro, int16_t adc){
  	
  	//Guardamos el valor que mide el sensor de iluminacion que se comunica con el pin A3
	int16_t  voltaje = (4096 / (pow(2, 15) - 1)) *  adc;

	//Mostrar por pantalla segun los valores de calibración
	if(voltaje <= oscuro) Serial.println("Es de noche o el sensor esta tapado."); 
	if(voltaje >= saturacion) Serial.println("Completamente soleado.");
	if(voltaje > oscuro && voltaje < ((saturacion-oscuro)/2)) Serial.println("Bastante nublado.");
	if(voltaje > ((saturacion-oscuro)/2) && voltaje < saturacion) Serial.println("Poco nublado.");
	 
}