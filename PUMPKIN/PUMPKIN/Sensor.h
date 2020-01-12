#ifndef SENSORES_H
#define SENSORES_H

#include "Arduino.h"
#include <Adafruit_ADS1015.h>


class Sensor {
  private:
    int pin;
    double min_valor;
    double power_pin;
    double lectura;
    double max_valor;
    Adafruit_ADS1115 ads1115;

  public:
    Sensor(int, double, double, double, Adafruit_ADS1115);
    Sensor(int, double, double, Adafruit_ADS1115);
    Sensor();

    void leerSensorInterruptor();
    void leerSensor();
    void obtenerVoltaje();
    void mostrarHumedad();
    void mostrarSalinidad();
    void mostrarIluminacion();
    void mostrarTemperatura();
 
};

#endif