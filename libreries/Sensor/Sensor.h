/*
 * Humedad.h - Libreria sensores
 */

 #ifndef SENSOR_h
 #define SENSOR_h

 #include "Arduino.h"
#include <Adafruit_ADS1015.h>

 class Sensor{

    private:
    	int pinADC;
    	double valor_min;
    	double valor_max;
    	int lectura;
    	int pin;
    	Adafruit_ADS1115 ads1115;

    public:
    	Sensor();
    	Sensor(int, double, double, Adafruit_ADS1115);
    	Sensor(int, double, double, int, Adafruit_ADS1115);
    	void lecturaHumedad();
    	void lecturaSalinidad();
    	void lecturaTemperatura(); 
 };
 #endif