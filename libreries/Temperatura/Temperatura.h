/*
 * Temperatura.h - Libreria para calcular Temperatura
 */

 #ifndef Temperatura_h
 #define Temperatura_h

#ifndef FICHERO_YA_INCLUIDO
#define FICHERO_YA_INCLUIDO

 #include "Arduino.h"

 class Temperatura{

    private:
        int ordenada;  // Ordenada calibrada
        int m;  // Pendiente calibrada
        int adcT; //pin al que esta conectado

    public:
    	Temperatura();
        int16_t getTemperatura();
      	void mensaje();
  
 }

 #endif