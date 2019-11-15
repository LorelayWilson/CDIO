/*
 * Salinidad.h - Libreria para calcular Salinidad
 */

 #ifndef Salindad_h
 #define Salinidad_h

#ifndef FICHERO_YA_INCLUIDO
#define FICHERO_YA_INCLUIDO

 #include "Arduino.h"

 class Salinidad{

    private:
        int noSalineValue;  // Medimos valor en seco
        int maxSalineValue;  // Medimos valor en agua
        int adcS; //
        int pin;
        int S = 600; //constante de margen de error

    public:
    	Salinidad();
        int16_t leerADC();
        int16_t getSalinidad();
      	void mensajeError();
  
 }

 #endif