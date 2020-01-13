#ifndef SENSORES_H
#define SENSORES_H

#include "Arduino.h"
#include <Adafruit_ADS1015.h>

class Sensores {
  private:
    int pin;
    int power_pin;
    double min_valor;
    double max_valor;
    int16_t lectura;
    Adafruit_ADS1115 ads1115;

  public:
    Sensores();
    Sensores(int, double, double, Adafruit_ADS1115);
    Sensores(int, int, double, double, Adafruit_ADS1115);

    void leerSensorInterruptor()();
    void leerSensor();
    void obtenerVoltaje();
    void medirHumedad();
    void medirSalinidad();
    void medirTemperatura();
    void medirLuz();
};

#endif