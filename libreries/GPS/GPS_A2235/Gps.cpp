
#include "gps.h"

TinyGPSPlus gps; // Definimos el objeto gps

SoftwareSerial ss(RX_PIN,TX_PIN); // Creamos una UART software para comunicaciOn con el GPS

/////////////////////////////////////////////////////////////////////////////////////////
//                                     FUNCIONES                                       //
/////////////////////////////////////////////////////////////////////////////////////////

// Funcion espera 1s para leer del GPS
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
}//()

// Funcion para encender/apagar mediante un pulso
void switch_on_off()
{
   // Power on pulse. Sin esto, el gps no se encenderia.
  digitalWrite(INIT_PIN,LOW);
  delay(200);
  digitalWrite(INIT_PIN,HIGH);
  delay(200); 
  digitalWrite(INIT_PIN,LOW);
 }//()


/////////////////////////////////////////////////////////////////////////
//                           CONFIGURACION                             //
/////////////////////////////////////////////////////////////////////////

void startGps() {
	
  Serial.begin(9600); // Inicializar la comunicacion con el monitor serie
  ss.begin(GPS_BAUD); // Inicializar la comunicacion con el GPS
  
  pinMode(INIT_PIN,OUTPUT); 
  switch_on_off(); // Pulso para encender el GPS
  
  //'Menu' de inicio de programa. Abrir la terminal antes de compilar para poder verlo.
  Serial.println("Latitud   Longitud   Alt");
  Serial.println("(deg)     (deg)      (ft)");
  Serial.println("-------------------------------------------------------------------------"); 
}//()

/////////////////////////////////////////////////////////////////////////
//                              BUCLE                                 //
////////////////////////////////////////////////////////////////////////

void mostrarGps() {

  if(gps.location.isValid()){ // Si el GPS esta recibiendo los mensajes NMEA

    Serial.print(gps.location.lat(),6); //Imprimir la latitid con 6 decimales.
    Serial.print(' ');
    Serial.print(' ');
    Serial.print(gps.location.lng(),6); //Imprimir la longitud con 6 decimales.
    Serial.print(' ');
    Serial.print(' ');
    Serial.print(gps.altitude.feet()); //Imprimir los pies de altitud.
  
  }else{  // Si no recibe los mensajes
  
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  
  smartDelay(1000);
}//()
