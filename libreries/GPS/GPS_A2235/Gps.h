
/////////////////////////
//                     //
//        gps.h        // 
//                     //         
/////////////////////////

#ifndef GPS_h
#define GPS_h
#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h> 

static void smartDelay(unsigned long, SofwareSerial);
void switch_on_off(int);
void startGps(int, int);
void mostrarGps(SoftwareSerial);

#endif
