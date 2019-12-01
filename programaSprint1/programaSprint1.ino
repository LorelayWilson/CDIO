//Autores: Iván Villanueva Villalón, Ferrán Almiñana Marchirant, Lorena Ioana Florescu y Raúl de la Fe Robles
#include <Wire.h>
#include <math.h>
#include "Sensor.h"
#include "IdentificarPersonas.h"
#include <EEPROM.h>
#include <Adafruit_ADS1015.h>
/*#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>*/

/****************************************************************************************************************************************
 *********************************************************CONSTANTES DEL PROGRAMA********************************************************
 *                                                 Estos se deben cambiar según los valores
 *                                                 calibrados de cada sensor en cuestion
 *                                                 Calibra primero tus sensores, y modifica
 *                                                 estas constantes según tu calibracion
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

  //CONSTANTES SENSOR ILUMINACION 
  /*const double B  = 790;  //ordenada en el origen
  const double M = 34.9; //  pendiente_calibrado
  const int ADCL = 2;  // Pin de entrada analogica para sensor temperatura*/

  //CAMBIAR SEGUN LA DIRECCION DE MEMORIA QUE USAS
  Adafruit_ADS1115 ads1115(0x48); // Creamos una dirección de memoría para la Ads1115 en la dirección 0x48
  
  //VARIABLES GLOBALES
  bool flag = false;
  int multiplo=1;
  int numOfInterrupts=0;
  volatile byte interruptCounter = 0;
  
/****************************************************************************************************************************************
 *ACELEROMETRO
 ****************************************************************************************************************************************/

#define MPU9250_ADDRESS            0x68
#define MAG_ADDRESS                0x0C
 
#define GYRO_FULL_SCALE_250_DPS    0x00  
#define GYRO_FULL_SCALE_500_DPS    0x08
#define GYRO_FULL_SCALE_1000_DPS   0x10
#define GYRO_FULL_SCALE_2000_DPS   0x18
 
#define ACC_FULL_SCALE_2_G        0x00  
#define ACC_FULL_SCALE_4_G        0x08
#define ACC_FULL_SCALE_8_G        0x10
#define ACC_FULL_SCALE_16_G       0x18

#define PWR_MGMT_1              0x6B
#define PWR_MGMT_2              0x6C
#define ACCEL_CONFIG_2          0x1D
#define INT_ENABLE              0x38 
#define MOST_DETECT_CTRL        0x69
#define WOM_THR                 0x1F
#define LP_ACCEL_ODR            0x1E

#define interruptPin 4 //pin de interrupcion -> 4

//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}

void interrupcion(){
  interruptCounter++;
}

void configurarAcelerometro(){
  Wire.begin();
   // Configurar acelerometro
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);

   //*ESTO ES LA TABLA QUE HEMOS HECHO* 0000 0000
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 0);
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_2, 7);
   I2CwriteByte(MPU9250_ADDRESS,ACCEL_CONFIG_2 ,5 );
   I2CwriteByte(MPU9250_ADDRESS,INT_ENABLE ,64 );
   I2CwriteByte(MPU9250_ADDRESS,0x37 ,128 ); 
   I2CwriteByte(MPU9250_ADDRESS,MOST_DETECT_CTRL ,192 );
   I2CwriteByte(MPU9250_ADDRESS,LP_ACCEL_ODR ,1 ); 
   I2CwriteByte(MPU9250_ADDRESS,WOM_THR ,2 ); 
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 32); 
   
   // Configurar interrupcion
   pinMode(interruptPin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(interruptPin), interrupcion , FALLING);

   
}

void lecturaAcelerometro(){

   // ---  Lectura acelerometro y giroscopio --- 
   uint8_t Buf[14];
   int FS_ACC = 16;
   int FS_GYRO = 2000;

   I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
 
   // Convertir registros acelerometro
   float ax = (Buf[0] << 8 | Buf[1]);
   float ay = (Buf[2] << 8 | Buf[3]);
   float az = Buf[4] << 8 | Buf[5];

   ax = (ax*FS_ACC/32768) - 25.57;
   ay = (ay*FS_ACC/32768) - 31.78;
   az = (az*FS_ACC/32768) + 0.94;

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




 
/****************************************************************************************************************************************
 *FUNCION MENU DEL PROGRAMA
 ****************************************************************************************************************************************/
void menuSensores(){

  //Numero sera el numero del personal. Ultima casilla sera la ultima posicion que esta vacia en la memoria EEPROM
  int numero, ultima_casilla;
  //Se guardara la lectura de los sensores
  int16_t lectura;
  if(flag){
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
            /*lectura = ads1115.readADC_SingleEnded(ADCT);
            medirTemperatura(B, M, lectura);
            flag = true;*/
            break;

          case 8:
            /*lectura = ads1115.readADC_SingleEnded(ADCT);
            medirTemperatura(B, M, lectura);*/
            flag = true;
            break;

          case 9:
            /*lectura = ads1115.readADC_SingleEnded(ADCT);
            medirTemperatura(B, M, lectura);*/
            flag = true;
            break;
            
          case 0:
            Serial.println("Introduzca su numero");
            delay(5000);
            numero = Serial.parseInt();
            mensaje(numero-1, 1);
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


      if (millis() > multiplo * 60000) {
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
//Función que se produce una sola vez para ajustar lo que necesitemos en el programa
void setup() {
  Serial.begin(9600); //Establecemos la velocidad de datos en 9600 baudios
  Wire.begin();
  ads1115.begin(); //Inicializamos el ADS1115
  ads1115.setGain(GAIN_ONE); //Ajustamos la ganancia a +/- 4.096V
  EEPROM.begin(512); //Inicializamos la memoria EEPROM
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

  //menuSensores();
  lecturaAcelerometro();
  delay (5000);
}
