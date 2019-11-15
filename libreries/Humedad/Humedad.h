/*
 * Humedad.h - Libreria para calcular Humedad
 */

 #ifndef Humedad_h
 #define Humedad_h

 #include "Arduino.h"

 class Humedad{

    private:
        int airValue = 20200;  // Medimos valor en seco
        int waterValue = 10250;  // Medimos valor en agua
    public:
        int16_t medirHumedad();
      
  
 }