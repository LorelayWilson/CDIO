//Autores: Iván Villanueva Villalón, Ferrán Almiñana Marchirant, Lorena Ioana Florescu y Raúl de la Fe Robles
#include <Wire.h>
#include <math.h>
#include "Sensor.h"
//#include "Gps.h"
#include "IdentificarPersonas.h"
#include <EEPROM.h>
#include <Adafruit_ADS1015.h>
#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>

/****************************************************************************************************************************************
 *********************************************************CONSTANTES DEL PROGRAMA********************************************************
 *                                                 Estos se deben cambiar según los valores                                             *
 *                                                 calibrados de cada sensor en cuestion                                                *
 *                                                 Calibra primero tus sensores, y modifica                                             *
 *                                                 estas constantes según tu calibracion                                                *
 ****************************************************************************************************************************************/

  //CONSTANTES SENSOR HUMEDAD
  const double AIR_VALUE = 20200;  // Medimos valor minimo de humedad (valor en seco)
  const double WATER_VALUE = 10250;  // Medimos valor maximo de humedad (valor en agua)
  const int ADCH = 0;  // Pin de entrada analogica para sensor humedad
 
  //CONSTANTES SENSOR SALINIDAD
  const double MIN_SALINIDAD = 3111; //Valor de la medida del sensor en agua destilada sin nada
  const double MAX_SALINIDAD = 22873; //Valor de la medida del sensor en agua destilada con la máxima cantidad de sal
  const int ADCS = 1;  // Pin de entrada analogica para sensor salinidad
  const int PIN_SAL = 5; // Pin I/O digital para salinidad
  
  //CONSTANTES SENSOR TEMPERATURA
  const double B  = 790;  //ordenada en el origen
  const double M = 34.9; //  pendiente_calibrado
  const int ADCT = 2;  // Pin de entrada analogica para sensor temperatura

  //CONSTANTES SENSOR LUZ
  const int ADCL = 3;  //Pin de entrada analógica para el sensor de salinidad
  const int MIN_LUZ = 25; //calibración con mínima luz (oscuridad)
  const int MAX_LUZ = 1800; //calibración con máxima luz (saturado)

  //CONSTANTES GPS
  unsigned long ms;

  //CONSTANTES ACELEROMETRO
  const byte interruptPin = 4; //pin de interrupcion -> 4

  //CAMBIAR SEGUN LA DIRECCION DE MEMORIA QUE USAS
  Adafruit_ADS1115 ads1115(0x48); // Creamos una dirección de memoría para la Ads1115 en la dirección 0x48
  
  //VARIABLES GLOBALES
  bool flag = false;
  int multiplo=1;
  int numOfInterrupts=0;
  volatile byte interruptCounter=0;
  
/****************************************************************************************************************************************
 *ACELEROMETRO
 ****************************************************************************************************************************************/

#define MPU9250_ADDRESS           0x68
#define MAG_ADDRESS               0x0C
 
#define ACC_FULL_SCALE_2_G        0x00  
#define ACC_FULL_SCALE_4_G        0x08
#define ACC_FULL_SCALE_8_G        0x10
#define ACC_FULL_SCALE_16_G       0x18

#define PWR_MGMT_1                0x6B
#define PWR_MGMT_2                0x6C
#define ACCEL_CONFIG_2            0x1D
#define INT_ENABLE                0x38 
#define MOT_DETECT_CTRL           0x69
#define WOM_THR                   0x1F
#define LP_ACCEL_ODR              0x1E

/*Funcion auxiliar lectura
 * @param Address
 * @param Register
 * @param Nbytes
 * @param Data
 */
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
/*Funcion auxiliar de escritura
 * @param Address
 * @param Register
 * @param Data
 */
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}

/*
 * Funcion para utilizar en el handleInterrupt
 */
void interrupcion(){
  interruptCounter++;
}

/*
 * Función para configurar el acelerometro
 */
void configurarAcelerometro(){
    Wire.begin();
   // Configurar acelerometro
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);

   //Configurar giroscopio
   I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);

   //*ESTO ES LA TABLA QUE HEMOS HECHO*
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 0);
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_2, 7);
   I2CwriteByte(MPU9250_ADDRESS,ACCEL_CONFIG_2 ,9 );
   I2CwriteByte(MPU9250_ADDRESS,INT_ENABLE ,64 );
   I2CwriteByte(MPU9250_ADDRESS,0x37, 128); 
   I2CwriteByte(MPU9250_ADDRESS,MOT_DETECT_CTRL ,192 );
   I2CwriteByte(MPU9250_ADDRESS,LP_ACCEL_ODR ,1 ); 
   I2CwriteByte(MPU9250_ADDRESS,WOM_THR ,2 ); 
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 32); 

   // Configurar interrupcion
   pinMode(interruptPin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(interruptPin), interrupcion , CHANGE);
}

/*
 * Funcion para sacar por pantalla los datos del acelerometro
 */
void lecturaAcelerometro(){

   // ---  Lectura acelerometro y giroscopio --- 
   uint8_t Buf[14];
   int FS_ACC = 16;
   int FS_GYRO=2000;

   I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
 
   // Convertir registros acelerometro
   float ax = (Buf[0] << 8 | Buf[1]);
   float ay = (Buf[2] << 8 | Buf[3]);
   float az = Buf[4] << 8 | Buf[5];

   ax = (ax*FS_ACC/32768) - 0.24;
   ay = (ay*FS_ACC/32768) - 0.24;
   az = (az*FS_ACC/32768) - 7.37;

   // Acelerometro
   Serial.println("Lectura Acelerometro");
   Serial.print("AX=");
   Serial.print(ax, 2);
   Serial.print("g");
   Serial.print("\t");
   Serial.print("AY=");
   Serial.print(ay, 2);
   Serial.print("g");
   Serial.print("\t");
   Serial.print("AZ=");
   Serial.print(az, 2);
   Serial.println("g"); 
}

/******************************************************************************
GPS A2235H 
Conectar RXI al pin 12 del ESP8266
Conectar TXO al pin 13 del ESP8266
Usamos el pin 15 para inicializar el GPS
******************************************************************************/

#define RX_PIN  12 // GPS RXI
#define TX_PIN  13 // GPS TX0
#define INIT_PIN 15 // Pin para  Inicializar el GPS
#define GPS_BAUD  4800  //  velocidad de comunicaciÃ³n serie

SoftwareSerial ss(RX_PIN, TX_PIN);
TinyGPSPlus gps; // Definimos el objeto gps

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
void switch_on_off(){
   // Power on pulse
  digitalWrite(INIT_PIN,LOW);
  delay(200);
  digitalWrite(INIT_PIN,HIGH);
  delay(200); 
  digitalWrite(INIT_PIN,LOW);
}

void configurarGps(){
  ss.begin(GPS_BAUD); // Inicializar la comunicación con el GPS
  pinMode(INIT_PIN,OUTPUT); 
  switch_on_off(); // Pulso para encender el GPS
}
 
void mostrarGps(){
  Serial.println("Fecha      Hora       Latitud   Longitud   Alt    Rumbo   Velocidad");
  Serial.println("(MM/DD/YY) (HH/MM/SS)     (deg)       (deg)  (ft)                   (mph)");
  Serial.println("-------------------------------------------------------------------------");

  char gpsDate[10]; 
  char gpsTime[10];

  if(gps.location.isValid()){ // Si el GPS está recibiendo los mensajes NMEA

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
    Serial.println(gps.speed.mph(),2);
  }
  else  // Si no recibe los mensajes
  {
    
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  smartDelay(1000);

   
  
}
/****************************************************************************************************************************************
 *FUNCION MENU DEL PROGRAMA
 ****************************************************************************************************************************************/

 void mostrarMenu() {
  Serial.println("Pulse 1 para identificarse");
  Serial.println("Pulse 2 para comprobar el registro de trabajadores");
  Serial.println("Pulse 3 para borrar el registro actual");
  Serial.println("Pulse 4 para ver la Humedad");
  Serial.println("Pulse 5 para ver la Salinidad");
  Serial.println("Pulse 6 para ver la Temperatura");
  Serial.println("Pulse 7 para ver la Iluminacion");
  Serial.println("Pulse 8 para ver datos del GPS");
  Serial.println("Pulse 9 para ver datos del Acelerómetro");
  Serial.println("Pulse 0 para acabar su jornada");
  Serial.println("Pulse f para finalizar el programa");
  delay(5000);

}

 /*
  * Función donde crearemos al menú
  */
void menuSensores(){

  //Numero sera el numero del personal. Ultima casilla sera la ultima posicion que esta vacia en la memoria EEPROM
  int numero, ultima_casilla;
  //Se guardara la lectura de los sensores
  int16_t lectura;
  if(flag){
      mostrarMenu();
      flag = false;
  }
  if(Serial.available()){
     char opc = Serial.read();
     if (opc >= '0' && opc <= '9')
      //restamos el valor '0' para obtener el numero enviado
        opc -= '0';
        
     //Quitamos los caracteres extra que se envian automaticamente en el monitor serie 
     if (opc != '\n' && opc != '\r') {
        switch (opc) {
         case 1:
            Serial.println("Introduzca su numero");
            delay(5000);
            numero = Serial.parseInt();
            //Funciones contenidas en Identificar Personas
            ultima_casilla = localizarUltimaCasilla();
            leerNumero(numero, ultima_casilla);
            mostrarNumero(ultima_casilla);
            mensaje(numero-1, 0);
            flag = true;
            break;

          case 2:
            Serial.println("El registro es: ");
            //Funciones contenidas en Identificar Personas
            ultima_casilla = localizarUltimaCasilla();
            comprobarRegistro(ultima_casilla);
            flag = true;
            break;

          case 3:
          //Funcion contenida en Identificar Personas
            inicializarMemoriaEEPROM();
            Serial.println("Memoria reiniciada");
            flag = true;
            break;
          case 4:
            lectura = ads1115.readADC_SingleEnded(ADCH);
            medirHumedad(AIR_VALUE, WATER_VALUE, lectura);
            flag = true;
            break;
          case 5:
            digitalWrite(PIN_SAL , HIGH);
            delay(100);
            lectura = ads1115.readADC_SingleEnded(ADCS);
            digitalWrite(PIN_SAL, LOW);
            delay(10);
            medirSalinidad(MIN_SALINIDAD, MAX_SALINIDAD, lectura);
            flag = true;
            break;
          
          case 6:
            lectura = ads1115.readADC_SingleEnded(ADCT);
            medirTemperatura(B, M, lectura);
            flag = true;
            break;
            
          case 7:
            lectura = ads1115.readADC_SingleEnded(ADCL);
            medirLuz(MAX_LUZ, MIN_LUZ, lectura);
            flag = true;
            break;

          case 8:

            mostrarGps();
            flag = true;
            break;

          case 9:
            lecturaAcelerometro();
            flag = true;
            break;
            
          case 0:
            Serial.println("Introduzca su numero");
            delay(5000);
            numero = Serial.parseInt();
            //Funcion contenida en Identificar Personas
            mensaje(numero-1, 1);
            flag = true;
            break;
            
          case 'f':
            Serial.println("El programa se detendrá");
            ESP.deepSleep(30000000);
            flag = true;
            break;
            
          default:
            Serial.println("Opcion no valida");
            flag = true;
        }
      }

      Serial.println();
      
      if (millis() > multiplo * 120000) {
        Serial.println("Pulse 1  si desea continuar utilizando el programa");
        Serial.println(" ");
        delay(5000);
        int x = Serial.parseInt();
        if (x != 1) {
          Serial.println("El programa se detendrá");
          Serial.println(" ");
          ESP.deepSleep(30000000);
        } else{
          multiplo++;
        }
      }
    }
}

/****************************************************************************************************************************************
 *FUNCION SETUP
 ****************************************************************************************************************************************/
/* 
 *  Función que se produce una sola vez para ajustar lo que necesitemos en el programa
 */
void setup() {
  Serial.begin(9600); //Establecemos la velocidad de datos en 9600 baudios
  ads1115.begin(); //Inicializamos el ADS1115
  ads1115.setGain(GAIN_ONE); //Ajustamos la ganancia a +/- 4.096V
  EEPROM.begin(512); //Inicializamos la memoria EEPROM
  flag = true;
  configurarAcelerometro();
  configurarGps();
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

  /*if(interruptCounter>0)
    Serial.println("es mayor");
  Serial.println(numOfInterrupts);
  lecturaAcelerometro();*/
  delay (5000);
}
