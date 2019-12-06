
#include "gps.h"

TinyGPSPlus gps; // Definimos el objeto gps

/////////////////////////////////////////////////////////////////////////////////////////
//                                     FUNCIONES                                       //
/////////////////////////////////////////////////////////////////////////////////////////

// Funcion espera 1s para leer del GPS
static void smartDelay(unsigned long ms, SofwareSerial ss){
	
  unsigned long start = millis();
  do {
    while(ss.available()){
      gps.encode(ss.read());  // leemos del gps
    }
  } while(millis() - start < ms);
}//()

// Funcion para encender/apagar mediante un pulso
void switch_on_off(int INIT_PIN){
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

void startGps(int GPS_BAUD, int INIT_PIN) {

  ss.begin(GPS_BAUD); // Inicializar la comunicacion con el GPS
  
  pinMode(INIT_PIN,OUTPUT); 
  switch_on_off(INIT_PIN); // Pulso para encender el GPS
  
}//()

/////////////////////////////////////////////////////////////////////////
//                              BUCLE                                 //
////////////////////////////////////////////////////////////////////////

void mostrarGps(SofwareSerial ss) {

	char gpsDate[100];
	char gpsTime[100];

  if(gps.location.isValid()){ // Si el GPS esta recibiendo los mensajes NMEA

  	 //'Menu' de inicio de programa. Abrir la terminal antes de compilar para poder verlo.
    Serial.println("Fecha      Hora       Latitud   Longitud   Alt    Rumbo   Velocidad");
    Serial.println("(MM/DD/YY) (HH/MM/SS)     (deg)       (deg)  (ft)                   (mph)");
    Serial.println("-------------------------------------------------------------------------");

	sprintf(gpsDate,"%d/%d/%d", gps.date.month(),gps.date.day(),gps.date.year()); // Construimos string de datos fecha
    sprintf(gpsTime,"%d/%d/0%d", gps.time.hour(),gps.time.minute(),gps.time.second());  // Construimos string de datos hora

    Serial.print(gpsDate);
    Serial.print('\t');
    Serial.print(gpsTime);
    Serial.print('\t');
    Serial.print(gps.location.lat(),6);
    Serial.print('\t');
    Serial.print(gps.location.lng(),6);
    Serial.print('\t');
    Serial.print(gps.altitude.feet());
    Serial.print('\t');
    Serial.print(gps.course.deg(),2);
    Serial.print('\t');
    Serial.println(gps.speed.mph(),2)
  
  }else{  // Si no recibe los mensajes
  
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  
  smartDelay(1000, ss);
}//()
