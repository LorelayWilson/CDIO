//Autores: Iván Villanueva Villalón, Ferrán Almiñana Marchirant, Lorena Ioana Florescu y Raúl de la Fe Robles
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <math.h>

Adafruit_ADS1115 ads1115(0x48); // Creamos una dirección de memoría para la Ads1115 en la dirección 0x48

  //Indicamos en que pin esta conectado el sensor de humedad y salinidad
  const int pin_sal = 5; // Pin I/O digital para salinidad
  
//Función que se produce una sola vez para ajustar lo que necesitemos en el programa
void setup() {

  Serial.begin(9600); //Establecemos la velocidad de datos en 9600 baudios
  pinMode(pin_sal, OUTPUT); //Configuramos el pin 5 como salida
  ads1115.begin(); //Inicializamos el ADS1115
  ads1115.setGain(GAIN_ONE); //Ajustamos la ganancia a +/- 4.096V

 
}

//Función loop donde se llamará a las funciones de los sensores
void loop() {

  int airValue = 20200;  // Medimos valor en seco
  int waterValue = 10250;  // Medimos valor en agua
  int noSalineValue = 3111; //Valor de la medida del sensor en agua destilada sin nada
  int maxSalineValue = 22873; //Valor de la medida del sensor en agua destilada con la máxima cantidad de sal
  int ordenada_calibrado = 790;
  int pendiente_calibrado = 34.9;


  int adcH = 0;
  int adcS = 1;
  int adcT = 2;
  
  //Muestra los valores de salinidad y humedad en porcentaje llamando a las funciones creadas para ello
  medirHumedad(airValue, waterValue, adcH);
  medirSalinidad(noSalineValue, maxSalineValue, adcS, pin_sal);
  medirTemperatura(ordenada_calibrado, pendiente_calibrado, adcT);
  Serial.println("");
  delay (5000);
}

//Función de humedad para calcular y mostrar por pantalla el porcentaje de humedad
void medirHumedad(int airValue, int waterValue, int adcH){

  int H = 600; //constante que usaremos como margen de error para humedad
  //Creamos las variables donde guardaremos el porcentaje de humedad y la lectura del sensor
  int16_t humedad, adc0;
  //Guardamos el valor que mide el sensor de humedad que se comunica con el pin A0
  adc0 = ads1115.readADC_SingleEnded(adcH);
  //Guardamos en humedad el calculo del porcentaje
  humedad = 100*airValue/(airValue-waterValue)-adc0*100/(airValue-waterValue);

  //Bucle que usamos para mandar mensaje de error si la lectura del adc0 no concuerda con la calibración
  if(adc0<(waterValue - H) or adc0>(airValue + H)) {
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
void medirSalinidad(int noSalineValue, int maxSalineValue, int adcS, int pin_sal){

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
  salinidad = ((adc1 - noSalineValue)* 100)/(maxSalineValue - noSalineValue);
  
  //Filtramos los valores obtenidos del sensor para que ver si se encuentran dentro de los límites de la calibración
  if(adc1<(noSalineValue - S) or adc1>(maxSalineValue + S)){
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

void medirTemperatura(int ordenada_calibrado, int pendiente_calibrado, int adcT){
    
  //Creamos las variables donde guardaremos el porcentaje d y la lectura del sensor
  int16_t temperatura, adc2, voltaje;
  
  //Guardamos el valor que mide el sensor de temperatura que se comunica con el pin A2
  adc2 = ads1115.readADC_SingleEnded(adcT);

  voltaje = (4.096/(pow(2,15)-1))*adc2;
   
  //Guardamos en humedad el calculo del porcentaje
  temperatura = (voltaje - ordenada_calibrado)/pendiente_calibrado;
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}
