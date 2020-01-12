/*
 * IdentificarPersonas.h - Libreria para Identificar Personas
 */

 #ifndef IdentificarPersonas_h
 #define IdentificarPersonas_h
 
 
#include <EEPROM.h>
#include "Arduino.h"

 void inicializarMemoriaEEPROM();
 void leerNumero(int, int );
 void mostrarNumero(int);
 int localizarUltimaCasilla();
 void comprobarRegistro(int);
 bool comprobarInicializacion();
 void mensaje(int, int);
 
 #endif