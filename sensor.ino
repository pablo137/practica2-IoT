#include <WiFi.h>
#include <PubSubClient.h>

#define echo 16
#define trigger 15
int cm = 0;

const char *WIFI_SSID = "LABO19";
const char *WIFI_PASS = "catolica19";

const char *SERVER_ADDRESS = "192.168.0.7"; //YOUR_SERVER_IP_ADDRESS mi maquina 192.168.78.48
const int SERVER_PORT = 5000;            //YOUR_SERVER_PORT

void setup() 
{
   Serial.begin(115200);
    
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);//Inicializamos el pin con 0
  
  
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  while(WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  
  Serial.print("\nÏP address: "); 
  Serial.println(WiFi.localIP());
}

  long readUltrasonicDistance(int triggerPin, int echoPin)
  {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    return pulseIn(echoPin, HIGH); //Se devolvera el tiempo entre el envio y la recepcion
  }

void loop() 
{ 
  delay(3000);
  cm = 0.01723 * readUltrasonicDistance(trigger,echo); //se calculara la distancia multiplicando la velocidad en la que el sonido recorre un centimetro por el tiempo de rebote obtenido.
  
  Serial.print("Connecting to: ");
  Serial.println(SERVER_ADDRESS);
  
  WiFiClient client;
  
  if(!client.connect(SERVER_ADDRESS, SERVER_PORT))
  {
    Serial.println("Connection failed!");
    delay(5000);
    reconect(); //va a volver a iniciar el codigo 
  }
            
  Serial.print(cm); //manda al serial los cm leidos
  Serial.print("\n"); 
  client.print(cm); //manda por internet al server los cm

  //Serial.println("Cerrando conection.");
  Serial.println("Closing connection.");
  client.stop(); 
}
}
