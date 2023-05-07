// C++ code
//CÓDIGO ESCLAVO
// Incluir librería Wire.h para usar la conexión I2C.
#include <Wire.h>

// Definición de variables
// Declaración del pin análogico A1 del arduino UNO como sensor_temp.
int sensor_temp = A1;
// Declaración de variable float para guardar el valor tomado por el sensor LM35.
float valor_sensor;
// Declaración de variable float para convertir el valor sensado a voltaje.
float voltaje;
// Declaración de variable float para convertir el valor del voltaje a grados Celsius.
float temp_convers;
// Declaración de varibale String para convertir los grados Celsius a tipo String.
String temp_string;

void setup()
{
  // Declarar el pin análogico de modo INPUT.
  pinMode(sensor_temp, INPUT);
  // Unirse al bus I2C en la dirección 11.
  Wire.begin(11);
  // Ejecutar la función requestFunc cuando el maestro este solicitando los datos del esclavo.
  Wire.onRequest(requestFunc);
  // Frecuencia para comenzar la comunicación serial.
  Serial.begin(9600);
}

void loop()
{
  // Leer la entrada en el pin análogo A1 (sensor_temp) y se guarda en la variable valor_sensor.
  valor_sensor = analogRead(sensor_temp);
  // Se convierte el valor sensado a voltaje (el sensado va de 0 - 1023) por lo cual se divide en 1023 y se multiplica por 5 porque se tiene como referencia de voltaje 5V.
  voltaje = valor_sensor * (5.0/1023.0);
  // Convertir el valor de voltaje a su correspondiente en temperatura multiplicando por 100.
  temp_convers = voltaje * 100;

  // Escritura de las diferentes variables por el monitor serial para verificar resultados.
  Serial.println("Log temperatura (esclavo):");
  Serial.println("__________");
  Serial.println("Analogo");
  Serial.println(valor_sensor);
  Serial.println("__________");
  Serial.println("Voltaje");
  Serial.println(voltaje);
  Serial.println("__________");
  Serial.println("Temperatura");
  Serial.print(temp_convers);
  Serial.println(" °C");

  // Convertir la temperatura en float a String para poder ser enviada al maestro.
  temp_string = String(temp_convers);

  // Retraso de 7 segundos.
  delay(7000);
}

// Función para enviar los datos al maestro cuando son solicitados por este.
void requestFunc(){
  // Enviar datos byte a byte al maestro con la función Wire.write
  Wire.write(temp_string[0]);
  Wire.write(temp_string[1]);
  Wire.write(temp_string[2]);
  Wire.write(temp_string[3]);
  Wire.write(temp_string[4]);
}
