#include <WiFi.h>

const char * WIFI_SSID = "2611PB";
const char * WIFI_PASS = "26110000";
const char * SERVER_ADDRESS = "192.168.43.123";
const int SERVER_PORT = 1234; 
const int TRIGGER_PIN = 15;
const int ECHO_PIN = 16;

class DistanceSensor {
public:
    long readDistance(int triggerPin, int echoPin) {
        pinMode(triggerPin, OUTPUT); 
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        pinMode(echoPin, INPUT);
        return pulseIn(echoPin, HIGH);
    }
};

void setup() {
    Serial.begin(115200);
    Serial.print("Connecting to: ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }

    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    DistanceSensor distanceSensor;
    long cm = 0.01723 * distanceSensor.readDistance(TRIGGER_PIN, ECHO_PIN);
    WiFiClient client;
    if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
        Serial.println("Connection Sensor to Server failed");
        return;
    }
    Serial.print(cm);
    Serial.println(" cm");
    delay(200);

    String set = "SET " + String(cm) + "\n";
    client.print(set.c_str()); 
    delay(200);
}
