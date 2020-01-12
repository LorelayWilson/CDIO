#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <ESP8266WiFi.h>
#include "Arduino.h"

class WifiConnection {
  private:
    const int LED_PIN = 5; 
    int cont = 0; 
    WiFiClient client;
  public:
    WifiConnection();
    void connectWiFi();
    bool conexion = false;
    void HTTPPost(String, int);
    void HTTPGet(String, int);
};

#endif /* WIFICONNECTION_H_ */
