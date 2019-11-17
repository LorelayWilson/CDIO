//Autores: Iván Villanueva Villalón, Ferrán Almiñana Marchirant, Lorena Ioana Florescu y Raúl de la Fe Robles
#include <Wire.h>
#include <math.h>
#include "Sensor.h"
#include "IdentificarPersonas.h"
#include <EEPROM.h>
int numero;
int ultima_casilla;
int menu;
int multiplo = 1;
bool flag = false;

/****************************************************************************************************************************************
 *********************************************************CONSTANTES DEL PROGRAMA********************************************************
 *                                                 Estos se deben cambiar según los valores
 *                                                 calibrados de cada sensor en cuestion
 *                                                 Calibra primero tus sensores, y modifica
 *                                                 estas constantes según tu calibracion
 ****************************************************************************************************************************************/

  //CONSTANTES SENSOR HUMEDAD
  const double MIN_HUMEDAD = 20200;  // Medimos valor minimo de humedad (valor en seco)
  const double MAX_HUMEDAD = 10250;  // Medimos valor maximo de humedad (valor en agua)
  const int adcH = 0;  // Pin de entrada analogica para sensor humedad

  //CONSTANTES SENSOR SALINIDAD
  const double MIN_SALINIDAD = 3111; //Valor de la medida del sensor en agua destilada sin nada
  const double MAX_SALINIDAD = 22873; //Valor de la medida del sensor en agua destilada con la máxima cantidad de sal
  const int adcS = 1;  // Pin de entrada analogica para sensor salinidad
  const int pin_sal = 5; // Pin I/O digital para salinidad
  
  //CONSTANTES SENSOR TEMPERATURA
  const double B  = 790;  //ordenada en el origen
  const double M = 34.9; //  pendiente_calibrado
  const int adcT = 2;  // Pin de entrada analogica para sensor temperatura
  

// Salinidad salinidad(noSalineValue, maxSalineValue, adcS, pin_sal);
// Temperatura temperatura(ordenada, m, adcT);



/****************************************************************************************************************************************
 *CREACION DE SENSORES
 ****************************************************************************************************************************************/

Adafruit_ADS1115 ads1115(0x48); // Creamos una dirección de memoría para la Ads1115 en la dirección 0x48
Sensor humedad;
Sensor salinidad;
Sensor temperatura;

void sensores(){
  Sensor hum(adcH, MIN_HUMEDAD, MAX_HUMEDAD, ads1115);
  Sensor sal(adcS, MIN_SALINIDAD, MAX_SALINIDAD, pin_sal, ads1115);
  Sensor temp(adcT, M, B, ads1115);
  humedad=hum;
  salinidad = sal;
  temperatura = temp;
}


/****************************************************************************************************************************************
 *FUNCION MENU DEL PROGRAMA
 ****************************************************************************************************************************************/

void menuSensores(){
  int16_t lectura;
 
  if(flag){
      Serial.println("Pulse 1 para identificarse");
      Serial.println("Pulse 2 para comprobar el registro de trabajadores");
      Serial.println("Pulse 3 para borrar el registro actual");
      Serial.println("Pulse 4 para ver la Temperatura");
      Serial.println("Pulse 5 para ver la Salinidad");
      Serial.println("Pulse 6 para ver la Humedad");
      Serial.println("Pulse 7 para acabar su jornada");
      Serial.println("Pulse 8 para finalizar el programa");
      delay(5000);
      flag = false;
  }
  if(Serial.available()){
     char opc = Serial.read();
     if (opc >= '1' && opc <= '9')
      //restamos el valor '0' para obtener el numero enviado
        opc -= '0';

     if (opc != '\n' && opc != '\r') {
        switch (opc) {
         case 1:
            Serial.println("Introduzca su numero");
            delay(5000);
            numero = Serial.parseInt();
            ultima_casilla = localizarUltimaCasilla();
            leerNumero(numero, ultima_casilla);
            mostrarNumero(ultima_casilla);
            mensaje(numero-1, 0);
            flag = true;
            break;

          case 2:
            Serial.print("El registro es: ");
            ultima_casilla = localizarUltimaCasilla();
            comprobarRegistro(ultima_casilla);
            flag = true;
            break;

          case 3:
            inicializarMemoriaEEPROM();
            Serial.println("Memoria reiniciada");
            flag = true;
            break;
          case 4:
            humedad.lecturaTemperatura();
            flag = true;
            break;
          case 5:
            humedad.lecturaSalinidad();
            flag = true;
            break;
          
          case 6:
            humedad.lecturaHumedad();
            flag = true;
            break;

          case 7:
            Serial.println("Introduzca su numero");
            delay(5000);
            numero = Serial.parseInt();
            mensaje(numero-1, 1);
            break;
            
          case 8:
            Serial.println("El programa se detendrá");
            ESP.deepSleep(30000000);
            flag = true;
            break;
          default:
            Serial.println("Opcion no valida");
            flag = true;
        }

     }
  }
}



/****************************************************************************************************************************************
 *FUNCION SETUP
 ****************************************************************************************************************************************/
//Función que se produce una sola vez para ajustar lo que necesitemos en el programa
void setup() {
  Serial.begin(9600); //Establecemos la velocidad de datos en 9600 baudios
  ads1115.begin(); //Inicializamos el ADS1115
  ads1115.setGain(GAIN_ONE); //Ajustamos la ganancia a +/- 4.096V
  EEPROM.begin(512);
  flag = true;
  sensores();
  if (comprobarInicializacion() == false) {
    inicializarMemoriaEEPROM();
  }
}

/****************************************************************************************************************************************
 *FUNCION MAIN
 ****************************************************************************************************************************************/
//Función loop donde se llamará a las funciones de los sensores
void loop() {

  menuSensores();
  delay (5000);
}
