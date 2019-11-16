/*
 * IdentificarPersonas.h - Libreria para Identificar Personas
 */

 #ifndef IdentificarPersonas_h
 #define IdentificarPersonas_h
 
 
#include <EEPROM.h>
 #include "Arduino.h"

 void inicializarMemoriaEEPROM();
 void leerNumero(int numero, int i);
 void mostrarNumero(int i);
 int localizarUltimaCasilla();
 void comprobarRegistro(int i);
 bool comprobarInicializacion();
 
 #endif