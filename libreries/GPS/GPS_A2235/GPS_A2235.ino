/******************************************************************************
GTI - Semestre 1A
GPS A2235H 
Conectar RXI al pin 12 del ESP8266
Conectar TXO al pin 13 del ESP8266
Usamos el pim 15 para inicializar el GPS
ATENCION: no poner el GPS sobre el Sparkfun 
******************************************************************************/

#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>
#define RX_PIN  12 // GPS RXI
#define TX_PIN  13 // GPS TX0
#define INIT_PIN 15 // Pin para  Inicializar el GPS


#define GPS_BAUD  4800  //  velocidad de comunicación serie 

TinyGPSPlus gps; // Definimos el objeto gps

SoftwareSerial ss(RX_PIN,TX_PIN); // Creamos una UART software para comunicación con el GPS

/////////////////////////////////////////////////////////////////////////////////////////
///// FUNCIONES

// Función espera 1s para leer del GPS
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while(ss.available())
    {
      gps.encode(ss.read());  // leemos del gps
    }
  } while(millis() - start < ms);
}
// Función para encender/apagar mediante un pulso
void switch_on_off()
{
   // Power on pulse. Sin esto, el gps no se encenderia.
  digitalWrite(INIT_PIN,LOW);
  delay(200);
  digitalWrite(INIT_PIN,HIGH);
  delay(200); 
  digitalWrite(INIT_PIN,LOW);
 }


/////////////////////////////////////////////////////////////////////////
// CONFIGURACION
////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); // Inicializar la comunicación con el monitor serie
  ss.begin(GPS_BAUD); // Inicializar la comunicación con el GPS
 
  pinMode(INIT_PIN,OUTPUT); 
  switch_on_off(); // Pulso para encender el GPS
  //'Mune' de inicio de programa. Abrir la terminal antes de compilar para poder verlo.
  Serial.println("Latitud   Longitud   Alt");
  Serial.println("(deg)     (deg)      (ft)");
  Serial.println("-------------------------------------------------------------------------"); 
}

/////////////////////////////////////////////////////////////////////////
// BUCLE
////////////////////////////////////////////////////////////////////////
void loop() {

  if(gps.location.isValid()){ // Si el GPS está recibiendo los mensajes NMEA

    Serial.print(gps.location.lat(),6); //Imprimir la latitid con 6 decimales.
    Serial.print(' ');
    Serial.print(' ');
    Serial.print(gps.location.lng(),6); //Imprimir la longitud con 6 decimales.
    Serial.print(' ');
    Serial.print(' ');
    Serial.print(gps.altitude.feet()); //Imprimir los pies de altitud.
  }
  else  // Si no recibe los mensajes
  {
    
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  smartDelay(1000);
}
