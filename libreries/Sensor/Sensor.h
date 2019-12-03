
//==============================//
// Sensor.h - Libreria sensores //
//==============================//


 #ifndef SENSOR_h
 #define SENSOR_h
 #include "Arduino.h"


void medirHumedad(int, int, int16_t);
void medirSalinidad(int, int, int16_t);
void medirTemperatura(int, int, int16_t); 
void medirLuz(int, int, int);

 #endif
