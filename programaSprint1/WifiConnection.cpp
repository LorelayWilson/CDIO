/*#ifndef WIFICONNECTION_H_
#define WIFICONNECTION_H_

#include "WifiConnection.h"

// Comentar/Descomentar para ver mensajes de depuracion en monitor serie y/o respuesta del HTTP server
#define PRINT_DEBUG_MESSAGES
//#define PRINT_HTTP_RESPONSE

//#define PRINT_HTTP_RESPONSE

// Comentar/Descomentar para conexion Fuera/Dentro de UPV
//#define WiFi_CONNECTION_UPV

// Selecciona que servidor REST quieres utilizar entre ThingSpeak y Dweet
//#define REST_SERVER_THINGSPEAK //Selecciona tu canal para ver los datos en la web (https://thingspeak.com/channels/360979)
#define REST_SERVER_DWEET //Selecciona tu canal para ver los datos en la web (http://dweet.io/follow/PruebaGTI)


bool conexion = false;
int cont = 0;*/

/*//////////////////////////////////////////////////////
Comentar/Descomentar para conexion Fuera/Dentro de UPV

#ifdef WiFi_CONNECTION_UPV //Conexion UPV
  const char WiFiSSID[] = "GTI1";
  const char WiFiPSK[] = "1PV.arduino.Toledo";
#else //Conexion fuera de la UPV
  const char WiFiSSID[] = "ADAMO-201D";
  const char WiFiPSK[] = "ZBU4HWWTD36CBM";
#endif
/////////////////////////////////////////////////////*/

  const char WiFiSSID[] = "ADAMO-201D";
  const char WiFiPSK[] = "ZBU4HWWTD36CBM";

/*//////////////////////////////////////////////////////
Selecciona que servidor REST quieres utilizar entre ThingSpeak y Dweet

//#define REST_SERVER_THINGSPEAK //Selecciona tu canal para ver los datos en la web (https://thingspeak.com/channels/360979)
#define REST_SERVER_DWEET //Selecciona tu canal para ver los datos en la web (http://dweet.io/follow/PruebaGTI)

#if defined(WiFi_CONNECTION_UPV) //Conexion UPV
  const char Server_Host[] = "proxy.upv.es";
  const int Server_HttpPort = 8080;
#elif defined(REST_SERVER_THINGSPEAK) //Conexion fuera de la UPV
  const char Server_Host[] = "api.thingspeak.com";
  const int Server_HttpPort = 80;
#else
  const char Server_Host[] = "dweet.io";
  const int Server_HttpPort = 80;
#endif
/////////////////////////////////////////////////////*/

  const char Server_Host[] = "dweet.io";
  const int Server_HttpPort = 80;

/*//////////////////////////////////////////////////////

#ifdef REST_SERVER_THINGSPEAK
  const char Rest_Host[] = "api.thingspeak.com";
  String MyWriteAPIKey="UAMQRNT5GNVRW13I"; // Escribe la clave de tu canal ThingSpeak
#else
  const char Rest_Host[] = "dweet.io";
  String MyWriteAPIKey="cdiocurso2018g08"; // Escribe la clave de tu canal Dweet
#endif

#define NUM_FIELDS_TO_SEND 2 //Numero de medidas a enviar al servidor REST (Entre 1 y 8)

/////////////////////////////////////////////////////*/
 
/*const char Rest_Host[] = "dweet.io";
String MyWriteAPIKey="cdiocurso2018g08"; // Escribe la clave de tu canal Dweet
#define NUM_FIELDS_TO_SEND 2 //Numero de medidas a enviar al servidor REST (Entre 1 y 8)

WifiConnection::WifiConnection(){
}*/

/////////////////////////////////////////////////////
/////////////// WiFi Connection ////////////////
//////////////////////////////////////////////////////

/*void WifiConnection::connectWiFi(){
  byte ledStatus = LOW;

  #ifdef PRINT_DEBUG_MESSAGES
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
  #endif

  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    #ifdef PRINT_DEBUG_MESSAGES
       Serial.print(".");
    #endif
    delay(500);
    cont++;
    if (cont == 10 || cont == 20)
       Serial.println("");
    if (cont == 30)
      return;
    
  }
  #ifdef PRINT_DEBUG_MESSAGES
     Serial.println( "WiFi Connected" );
     Serial.println(WiFi.localIP()); // Print the IP address
     conexion = true;
  #endif
}*/

/////////////////////////////////////////////////////
/////////////// HTTP POST  ThingSpeak////////////////
//////////////////////////////////////////////////////

//void WifiConnection::HTTPPost(String fieldData, int numFields){

// Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP POST
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar numFields al n�mero adecuado de datos que necesitas enviar y activa los campos en tu canal web

   /* if (client.connect( Server_Host , Server_HttpPort )){

        // Construimos el string de datos. Si tienes multiples campos asegurate de no pasarte de 1440 caracteres

        String PostData= "api_key=" + MyWriteAPIKey ;
        for ( int field = 1; field < (numFields + 1); field++ ){
            PostData += "&field" + String( field ) + "=" + fieldData[ field ];
        }*/

        // POST data via HTTP
       /* #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( "Connecting to ThingSpeak for update..." );
        #endif
        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );
        #ifdef PRINT_DEBUG_MESSAGES
            Serial.println( PostData );
            Serial.println();
            //Para ver la respuesta del servidor
            #ifdef PRINT_HTTP_RESPONSE
              delay(500);
              Serial.println();
              while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
              Serial.println();
              Serial.println();
            #endif
        #endif
    }
}*/

////////////////////////////////////////////////////
/////////////// HTTP GET  ////////////////
//////////////////////////////////////////////////////

//void WifiConnection::HTTPGet(String fieldData, int numFields){

// Esta funcion construye el string de datos a enviar a ThingSpeak o Dweet mediante el metodo HTTP GET
// La funcion envia "numFields" datos, del array fieldData.
// Asegurate de ajustar "numFields" al n�mero adecuado de datos que necesitas enviar y activa los campos en tu canal web

    /*if (client.connect( Server_Host , Server_HttpPort )){
           #ifdef REST_SERVER_THINGSPEAK
              String PostData= "GET https://api.thingspeak.com/update?api_key=";
              PostData= PostData + MyWriteAPIKey ;
           #else
              String PostData= "GET http://dweet.io/dweet/for/";
              PostData= PostData + MyWriteAPIKey +"?" ;
           #endif

           for ( int field = 1; field < (numFields + 1); field++ ){
              PostData += "&field" + String( field ) + "=" + fieldData[ field ];
           }


           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( "Connecting to Server for update..." );
           #endif
           client.print(PostData);
           client.println(" HTTP/1.1");
           client.println("Host: " + String(Rest_Host));
           client.println("Connection: close");
           client.println();
           #ifdef PRINT_DEBUG_MESSAGES
              Serial.println( PostData );
              Serial.println();
              //Para ver la respuesta del servidor
              #ifdef PRINT_HTTP_RESPONSE
                delay(500);
                Serial.println();
                while(client.available()){String line = client.readStringUntil('\r');Serial.print(line); }
                Serial.println();
                Serial.println();
              #endif
           #endif
    }
}*/
/*
bool conectado(){
  return conexion;
}
*/
//#endif /* WIFICONNECTION_H_ */
