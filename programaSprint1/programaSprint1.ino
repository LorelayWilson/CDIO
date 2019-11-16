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
  const double ORDENADA  = 790;  //ordenada en el origen
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
  Sensor sal(adcS, MIN_SALINIDAD, MAX_SALINIDAD, ads1115);
  Sensor temP(adcT, M, ORDENADA, ads1115);
  humedad=hum;
  salinidad = sal;
  temperatura = temP;
}


/****************************************************************************************************************************************
 *FUNCION MENU DEL PROGRAMA
 ****************************************************************************************************************************************/

void menuSensores(){
  int16_t lectura;

  if (flag) {
      Serial.println("Pulse 1 para identificarse");
      Serial.println("Pulse 2 para comprobar el registro de trabajadores");
      Serial.println("Pulse 3 para borrar el registro actual");
      Serial.println("Pulse 4 para ver la Temperatura");
      Serial.println("Pulse 5 para ver la Salinidad");
      Serial.println("Pulse 6 para ver la Humedad");
      Serial.println("Pulse 7 para finalizar el programa");
      delay(5000);
      flag = false;
  }
  if(Serial.available()){
     char opc = Serial.read();
     if (opc >= '1' && opc <= '9')
      //restamos el valor '0' para obtener el numeroenviado
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
            Serial.println(" ");
            break;

          case 2:
            Serial.print("El registro es: ");
            ultima_casilla = localizarUltimaCasilla();
            comprobarRegistro(ultima_casilla);
            Serial.println(" ");
            break;

          case 3:
            inicializarMemoriaEEPROM();
            Serial.println("Memoria reiniciada");
            Serial.println(" ");
            break;
          case 4:
            humedad.lecturaTemperatura();
            Serial.println(" ");
            break;
          case 5:
            humedad.lecturaSalinidad();
            Serial.println(" ");
            break;
          
          case 6:
            humedad.lecturaHumedad();
            Serial.println(" ");
            break;
    
          case 7:
            Serial.println("El programa se detendrá");
            Serial.println(" ");
            ESP.deepSleep(30000000);
            break;
           
        }
        flag = true;
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
  sensores();
  flag = true;
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
  Serial.println("");
  delay (5000);
}

/*
//Función de humedad para calcular y mostrar por pantalla el porcentaje de humedad
void medirHumedad(int airValue, int waterValue, int adcH) {

  int H = 600; //constante que usaremos como margen de error para humedad
  //Creamos las variables donde guardaremos el porcentaje de humedad y la lectura del sensor
  int16_t humedad, adc0;
  //Guardamos el valor que mide el sensor de humedad que se comunica con el pin A0
  adc0 = ads1115.readADC_SingleEnded(adcH);
  //Guardamos en humedad el calculo del porcentaje
  humedad = 100 * airValue / (airValue - waterValue) - adc0 * 100 / (airValue - waterValue);

  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if (adc0 < (waterValue - H) or adc0 > (airValue + H)) {
    Serial.print("Error lectura humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  } else {
    Serial.print("La humedad es: ");
    Serial.print(humedad);
    Serial.println("%");
  }
}


//Descripción de la función: muestra por pantalla el porcentaje de salinidad
void medirSalinidad(int noSalineValue, int maxSalineValue, int adcS) {

  int S = 500; //constante que usaremos como margen de error para salinidad

  //Creamos variables donde guardaremos el porcentaje de salinidad y la lectura del sensor
  int16_t salinidad, adc1;

  digitalWrite( pin_sal, HIGH );
  delay(100);

  //Guardamos el valor que mide el sensor de salinidad que se comunica con el pin A1
  adc1 = ads1115.readADC_SingleEnded(adcS);
  digitalWrite( pin_sal, LOW );
  delay(10);

  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  salinidad = ((adc1 - noSalineValue) * 100) / (maxSalineValue - noSalineValue);

  //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if (adc1 < (noSalineValue - S) or adc1 > (maxSalineValue + S)) {
    Serial.print("Error de lectura salinidad: ");
    Serial.print("La salinidad es: ");
    Serial.print(salinidad);
    Serial.println(" %");

  } else {
    Serial.print("La salinidad es: ");
    Serial.print(salinidad);
    Serial.println(" %");
  }
}


void medirTemperatura(int ordenada_calibrado, int pendiente_calibrado, int adcT) {

  //Creamos las variables donde guardaremos el porcentaje d y la lectura del sensor
  int16_t temperatura, adc2, voltaje;

  //Guardamos el valor que mide el sensor de temperatura que se comunica con el pin A2
  adc2 = ads1115.readADC_SingleEnded(adcT);

  voltaje = (4096 / (pow(2, 15) - 1)) * adc2;

  //Guardamos en humedad el calculo del porcentaje
  temperatura = (voltaje - ordenada_calibrado) / pendiente_calibrado;
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}

void identificarPersonas () {
  String nombres[] = {"Ivan", "Raul", "Ferran", "Lorena", "Asun", "Pepe"};
  int i = 0, j = 0, t = 0;
  String mensaje[] = {"Bienvenido/a, ", "Hasta luego, "};

  Serial.println("Introduzca su numero de identificacion");

  while(!Serial.available());
  
  if(Serial.available()) {
    int numero = Serial.parseInt();

    Serial.print("Su numero de identificación es: ");
    Serial.println(numero);

    while (EEPROM.read(i) != 0) {
      i++;
    }
    EEPROM.write(i, numero);
    EEPROM.commit();

    for (int j = 0; j <= i; j++) {
      if (EEPROM.read(j) == numero)
        t++;
    }
    if (t % 2 != 0 && t != 0) {
      Serial.print(mensaje[0]);
      Serial.print(nombres[numero - 1]);
      Serial.println(".");
    }

    if (t % 2 == 0 && t != 0) {
      Serial.print(mensaje[1]);
      Serial.print(nombres[numero - 1]);
      Serial.println(".");
    }
  }
}
*/
