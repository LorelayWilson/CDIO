
//==============================//
// Sensor.h - Libreria sensores //
//==============================//


 #ifndef SENSOR_h
 #define SENSOR_h
 #include "Arduino.h"


void mostrarHumedad(int, int, int16_t);
void mostrarSalinidad(int, int, int16_t);
void mostrarTemperatura(int, int, int16_t); 
void mostrarLuz(int, int, int16_t);
int16_t getLuz(int16_t);
int16_t getTemperatura(int , int, int16_t);
int16_t getSalinidad(int, int, int16_t);
int16_t getHumedad(int, int, int16_t);

 #endif
