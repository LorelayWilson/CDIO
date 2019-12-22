#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <ESP8266WiFi.h>
#include "Arduino.h"

class WifiConnection {

	private:
		WiFiClient client;
		const int LED_PIN = 5;


	public:

		WifiConnection();
	    void connectWiFi();
	    bool conexion = false;
	    //bool conectado();
	    void HTTPPost(String, int);
	    void HTTPGet(String, int);

};

#endif