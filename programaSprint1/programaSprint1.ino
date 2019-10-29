//Autores: Iván Villanueva Villalón, Ferrán Almiñana Marchirant, Lorena Ioana Florescu y Raúl de la Fe Robles
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads1115(0x48); // Creamos una dirección de memoría para la Ads1115 en la dirección 0x48

//Función que se produce una sola vez para ajustar lo que necesitemos en el programa
void setup() {
  Serial.begin(9600); //Establecemos la velocidad de datos en 9600 baudios
  pinMode(pin_sal, OUTPUT); //Configuramos el pin 5 como salida
  ads1115.begin(); //Inicializamos el ADS1115
  ads1115.setGain(GAIN_ONE); //Ajustamos la ganancia a +/- 4.096V
  
}

//Función loop donde se llamará a las funciones de los sensores
void loop() {
  //Muestra los valores de salinidad y humedad en porcentaje llamando a las funciones creadas para ello
  medirHumedad();
  medirSalinidad();
  Serial.println("");
  delay (5000);
}

//Función de humedad para calcular y mostrar por pantalla el porcentaje de humedad
void medirHumedad(){

  int AirValue = 20200;  // Medimos valor en seco
  int WaterValue = 10250;  // Medimos valor en agua
  int H = 600; //constante que usaremos como margen de error para humedad
  //Creamos las variables donde guardaremos el porcentaje de humedad y la lectura del sensor
  int16_t humedad, adc0;
  
  //Guardamos el valor que mide el sensor de humedad que se comunica con el pin A0
  adc0 = ads1115.readADC_SingleEnded(0);
   
  //Guardamos en humedad el calculo del porcentaje
  humedad = 100*AirValue/(AirValue-WaterValue)-adc0*100/(AirValue-WaterValue);

  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if(adc0<(WaterValue - H) or adc0>(AirValue + H)) {
    Serial.print("Error lectura humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  } else{
    Serial.print("La humedad es: ");
    Serial.print(humedad);
    Serial.println("%");
    }
}
//Descripción de la función: muestra por pantalla el porcentaje de salinidad
void medirSalinidad(){

  int NoSalineValue = 3111; //Valor de la medida del sensor en agua destilada sin nada
  int MaxSalineValue = 22873; //Valor de la medida del sensor en agua destilada con la máxima cantidad de sal
  int S = 500; //constante que usaremos como margen de error para salinidad
  int pin_sal = 5; // Pin I/O digital
  //Creamos variables donde guardaremos el porcentaje de salinidad y la lectura del sensor
  int16_t salinidad, adc1;
  
  digitalWrite( pin_sal, HIGH ); 
  delay(100); 

  //Guardamos el valor que mide el sensor de salinidad que se comunica con el pin A1
  adc1 = ads1115.readADC_SingleEnded(1); 
  digitalWrite( pin_sal, LOW ); 
  delay(10); 

  //Aplicamos la siguiente fórmula para calcular el porcentaje de salinidad
  salinidad = ((adc1 - NoSalineValue)* 100)/(MaxSalineValue - NoSalineValue);
  
  //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if(adc1<(NoSalineValue - S) or adc1>(MaxSalineValue + S)){
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
