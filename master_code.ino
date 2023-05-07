// C++ code
//MASTER
// Librerías
// Incluir librería Wire.h para usar la conexión I2C.
#include <Wire.h>
#include <Arduino.h>
// Incluir librería WiFi.h para usar las funcionalidades de WiFi del esp32.
#include <WiFi.h>
// Incluir librería ThingSpeak.h para mandar los datos al dashboard de ThingSpeak.
#include "ThingSpeak.h"

// ID del canal creado en ThingSpeak.
#define CHANNEL_ID 2132927
// API KEY del canal en ThingSpeak para permitir la escritura o lectura en un canal privado.
#define CHANNEL_API_KEY "UP1OYSZUEIPST9RY"
//Tiempo máximo para conectarse a internet.
#define WIFI_TIMEOUT_MS 20000
// Nombre de la red a conectar.
#define WIFI_NETWORK ""
// Contraseña de la red a conectar.
#define WIFI_PASSWORD ""

// Instancia de WiFiClient para que ThingSpeak haga peticiones HTTP.
WiFiClient client;

// Declaración del pin GPIO5 del ESP32 como led_trigger.
int led_trigger = 5;

// Función para conectarse a WiFi
void connectTowiFi(){
  Serial.print("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  unsigned long startAttemptTime = millis();

// Intentar conectarse mientras no haya conexión hasta que se acabe el tiempo de conexión.
while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
  Serial.print(".");
  delay (100);
}

// Verificar que si se está conectado a internet.
if(WiFi.status() != WL_CONNECTED) {
  Serial.println("Failed!");
//Handle this case. Restart ESP, go to deep sleep, retry after delay. 
//eg: ESP.restart();
}else{
  Serial.print("Connected!");
  Serial.println(WiFi.localIP());
}
}

void setup()
{
  // Unirse al bus i2c, con los pines GPIO21 como SDA y GPIO22 como SCL.
  Wire.begin(21,22);
  // Declarar el pin GPIO5 de modo OUTPUT.
  pinMode(led_trigger, OUTPUT);
  // Frecuencia para comenzar la comunicación serial.
  Serial.begin(115200);
  // Llamada a la función para conectarse a WiFi.
  connectTowiFi();
  // Inicializar la conexión a ThingSpeak.
  ThingSpeak.begin(client);

}

void loop()
{
  // Variable String para guardar los caracteres que envia el esclavo.
  String str_temp = "";
  // Variale para convertir el String con la temperatura a floar para hacer comparaciones númericas.
  float temp_float = 0.0F;
  // Comenzar transmisión con dirección 11 para enviar datos al esclavo.
  Wire.beginTransmission(11);
  Wire.write("start");
  // Fin de la transmisión.
  Wire.endTransmission();
  // Función para solicitar datos del esclavo, por la dirección 11 y solicitar 8 bytes.
  Wire.requestFrom(11, 8);
  
  // Leer los datos enviados por el esclavo mientras haya conexión.
  while(Wire.available())
  {
    char c = Wire.read(); // Recibir cada byte como un char.
    str_temp = str_temp + c; // Añadir cada char a un String.
  }
  
  // Convertir el string recibido a tipo float.
  temp_float = str_temp.toFloat();
  
  // Escritura de las diferentes variables por el monitor serial para verificar resultados.
  Serial.println("__________");
  Serial.println("Log temperatura (maestro):");
  Serial.print(temp_float);
  Serial.println(" °C");

  // Lógica if para comparar la temperatura si es mayor a 30 grados Celsius.
  if(temp_float >= 30){
    // Si la temperatura es mayor o igual a 30 grados Celsius, el pin led trigger tendrá una salida de 5V.
    digitalWrite(led_trigger,HIGH);
  }
  else{
    // Si la temperatura es menor a 30 grados Celsius, el pin led trigger tendrá una salida de 0V.
    digitalWrite(led_trigger,LOW);
  }

  // Escribir en el canal de ThingSpeak de número CHANNEL_ID, en el campo 1, la variable temp_float, con la llave API CHANNEL_API_KEY.
  ThingSpeak.writeField(CHANNEL_ID, 1, temp_float, CHANNEL_API_KEY);
  // Retraso de 7 segundos.
  delay(7000);
}