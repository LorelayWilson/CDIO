
/////////////////////////
//                     //
//        gps.h        // 
//                     //         
/////////////////////////

#ifndef GPS_h
#define GPS_h
#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h> 

static void smartDelay(unsigned long);
void switch_on_off();
void startGps();
void mostrarGps();

#endif
