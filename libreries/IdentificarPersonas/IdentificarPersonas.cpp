
#include "IdentificarPersonas.h"

//Inicializamos la memoria EEPROM poniendo 0 en todas sus casillas
void inicializarMemoriaEEPROM(){
	for(int i = 0; i <= 511; i++){
		EEPROM.write(i,0);
		EEPROM.commit();
	}
}

//Guardamos el numero introducido por pantalla en la memoria EEPROM
void leerNumero(int numero, int i) {
	EEPROM.write(i,numero);
	EEPROM.commit();
}

//Función para mostrar por pantalla
void mostrarNumero(int i){
    Serial.println(EEPROM.read(i));
}

//Calculamos cual es la ultima casilla sin utilizar
int localizarUltimaCasilla(){
	for(int i = 0; i <= 511; i++){
		if(EEPROM.read(i) == 0)
			return i;
	}
}

//Mostramos todos los numeros contenidos en la memoria EEPROM diferentes de 0
void comprobarRegistro(int i){
	for(int j = 0; j <= i-1; j++){
		Serial.println(EEPROM.read(j));
	}
}

//Comprobamos si la memoria EEPROM ya ha sido inicializada
bool comprobarInicializacion(){
	for(int i = 0; i <= 511; i++){
		if(EEPROM.read(i) == 0)
			return true;
	}
	return false;
}

void mensaje(int i, int j){

	String nombres[] = {"Ivan", "Raul", "Ferran", "Lorena", "Asun", "Pepe"};
	String mensaje[] = {"Bienvenido/a, ", "Hasta luego, "};
	
	if(i<1 or i>6) //buscar algo parecido a nombres.length()

		  Serial.println("Numero de identificacion erroneo.");
 	else {

		Serial.print(mensaje[j]);
		Serial.print(nombres[i]);
		Serial.println(".");
	}

		

}
