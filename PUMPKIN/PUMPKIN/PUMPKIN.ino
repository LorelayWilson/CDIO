#include "Arduino.h"
#include <Wire.h>
#include "Sensor.h"
#include "WifiConnection.h"

int contador = 0;
/******************************************************************************
Constantes
******************************************************************************/
// CONSTANTES DE LA HUMEDAD
const double AIR_VALUE = 20200;  // Medimos valor minimo de humedad (valor en seco)
const double WATER_VALUE = 10250;  // Medimos valor maximo de humedad (valor en agua)
const int ADCH = 2;  // Pin de entrada analogica para sensor humedad

// CONSTANTES DE LA SALINIDAD
const double MIN_SALINIDAD = 3111; //Valor de la medida del sensor en agua destilada sin nada
const double MAX_SALINIDAD = 22873; //Valor de la medida del sensor en agua destilada con la máxima cantidad de sal
const int ADCS = 0;  // Pin de entrada analogica para sensor salinidad
const int PIN_SAL = 5; // Pin I/O digital para salinidad

//CONSTANTES DE LA TEMPERATURA
const double B  = 790;  //ordenada en el origen
const double M = 34.9; //  pendiente_calibrado
const int ADCT = 1;  // Pin de entrada analogica para sensor temperatura

//CONSTANTES DE ILUMINACION
const int ADCL = 3;  //Pin de entrada analógica para el sensor de salinidad
const int MIN_LUZ = 25; //calibración con mínima luz (oscuridad)
const int MAX_LUZ = 1800; //calibración con máxima luz (saturado)

/******************************************************************************
ACÉLEROMETRO 
******************************************************************************/

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
#define MOST_DETECT_CTRL           0x69
#define WOM_THR                   0x1F
#define LP_ACCEL_ODR              0x1E

#define interruptPin 4 //pin de interrupcion -> 4

int numOfInterrupts=0;
volatile byte interruptCounter = 0;

//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
 
// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}
 
void handleInterrupt(){
   interruptCounter++;
}

void configurarAcelerometro()
{
  Wire.begin();
  Serial.println("Configurando acelerómetro...");
   // Configurar acelerometro
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);

     //*ESTO ES LA TABLA QUE HEMOS HECHO*
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 0);
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_2, 7);
   I2CwriteByte(MPU9250_ADDRESS,ACCEL_CONFIG_2 ,5 );
   I2CwriteByte(MPU9250_ADDRESS,INT_ENABLE ,64 );
   I2CwriteByte(MPU9250_ADDRESS,0x37 ,128 ); //pin CFG
   I2CwriteByte(MPU9250_ADDRESS,MOST_DETECT_CTRL ,192 );
   I2CwriteByte(MPU9250_ADDRESS,LP_ACCEL_ODR ,1 ); //es la frecuencia
   I2CwriteByte(MPU9250_ADDRESS,WOM_THR ,2 ); //es la sensibilidad
   I2CwriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 32); //2^5

  // Configurar interrupcion
   pinMode(interruptPin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

}

void lecturaAcelerometro()
{
  // Interrupciones
   if (interruptCounter>0){
               interruptCounter--;   //para que vuelva a cero
               numOfInterrupts++;
       }
  
  // ---  Lectura acelerometro --- 
   uint8_t Buf[14];
   int FS_ACC = 16;

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
Usamos el pim 15 para inicializar el GPS
ATENCION: no poner el GPS sobre el Sparkfun 
******************************************************************************/

#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>

#define RX_PIN  12 // GPS RXI
#define TX_PIN  13 // GPS TX0
#define INIT_PIN 15 // Pin para  Inicializar el GPS

#define GPS_BAUD  4800  //  velocidad de comunicación serie 

TinyGPSPlus gps; // Definimos el objeto gps

SoftwareSerial ss(RX_PIN,TX_PIN); // Creamos una UART software para comunicación con el GPS


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
void switch_on_off()
{
   // Power on pulse
  digitalWrite(INIT_PIN,LOW);
  delay(200);
  digitalWrite(INIT_PIN,HIGH);
  delay(200); 
  digitalWrite(INIT_PIN,LOW);
}

void lecturaGPS(){
  char gpsDate[10]; 
  char gpsTime[10];
  if(gps.location.isValid()){ // Si el GPS está recibiendo los mensajes NMEA

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
    Serial.println(gps.speed.mph(),2);
  } else { // Si no recibe los mensajes
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
  }
  smartDelay(1000);
}

void conectarGPS(){
   ss.begin(GPS_BAUD); // Inicializar la comunicación con el GPS
 
  pinMode(INIT_PIN,OUTPUT); 
  switch_on_off(); // Pulso para encender el GPS
}


/******************************************************************************
Sensores
******************************************************************************/
// VARIABLES SENSORES
Adafruit_ADS1115 ads1115(0x48);
Sensor salinidad;
Sensor humedad;
Sensor temperatura;
Sensor iluminacion;
WifiConnection wifi;

void construirSensores(){
  Sensor _salinidad(ADCS, MAX_SALINIDAD, MIN_SALINIDAD, PIN_SAL, ads1115);
  salinidad = _salinidad;
  
  Sensor _humedad(ADCH, WATER_VALUE, AIR_VALUE, ads1115);
  humedad = _humedad;
  
  Sensor _temperatura(ADCT, B, M, ads1115);
  temperatura = _temperatura;

  Sensor _iluminacion(ADCL, MAX_LUZ, MIN_LUZ, ads1115);
  iluminacion = _iluminacion;
}


/******************************************************************************
Setup
******************************************************************************/
void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando...");
  ads1115.begin(); //Iniciar ads1115
  Serial.println("Ajustando la ganancia...");
  ads1115.setGain(GAIN_ONE);
  Serial.println("Tomando medidas del canal AIN0");
  Serial.println("Rango del ADC: +/- 4.096V (1 bit=2mV)");

  construirSensores();
  
  conectarGPS();

  configurarAcelerometro(); 
  
  Serial.println("Inicializamos el dispositivo wifi...");

  wifi.connectWiFi();
}

/******************************************************************************
Funcion para controlar los sensores
******************************************************************************/
void leerDatos() {
  // Control de la humedad
  // Muestra por pantalla los datos recogidos del sensor de humedad.
  Serial.println("\n----- HUMEDAD -----");
  humedad.mostrarHumedad();
  
  // Control de la salinidad
  // Muestra por pantalla los datos recogidos del sensor de salinidad.
  Serial.println("\n----- SALINIDAD -----");
  salinidad.mostrarSalinidad();

  // Control de temperatura
  // Muestra por pantalla los datos recogidos del sensor de temperatura.
  Serial.println("\n----- TEMPERATURA -----");
  temperatura.mostrarTemperatura();
  
  // Control de iluminación
  // Muestra por pantalla los datos recogidos del sensor de iluminación.
  Serial.println("\n----- ILUMINACION -----");
  iluminacion.mostrarIluminacion();
  
  // Acelerómetro
  // Muestra por pantalla los datos recogidos del GPS.
  Serial.println("\n------ ACELERÓMETRO -----");
  lecturaAcelerometro();

  // GPS
  // Muestra por pantalla los datos recogidos del GPS.
  Serial.println("\n----- GPS ------");
  lecturaGPS();

}

void loop() {
  Serial.print("\n\n-----Prueba ");
  Serial.print(contador++);
  if (wifi.conexion){
    Serial.println("\t\t\tConectado a GTI1! ");
  }else {
    Serial.println("");
  }

  String data[8];
  data[1] = String (random(10, 20));
  #ifdef PRINT_DEBUG_MESSAGES
    Serial.print("Random1= ");
    Serial.println(data[1]);
  #endif

  wifi.HTTPGet(data, 8);
  leerDatos();

  delay(1000);
}
