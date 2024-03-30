#include <WiFi.h>

const char *WIFI_SSID = "2611PB";
const char *WIFI_PASS = "26110000";
const char *SERVER_ADDRESS = "192.168.43.123";
const int SERVER_PORT = 1234;

class LEDController {
private:
    int leds[7];

public:
    LEDController(const int* ledPins) {
        for (int i = 0; i < 7; i++) {
            pinMode(ledPins[i], OUTPUT);
            leds[i] = ledPins[i];
        }
    }

    void updateLEDs(bool states[7]) {
        for (int i = 0; i < 7; i++) {
            digitalWrite(leds[i], states[i]);
        }
    }
};


class WiFiClientController {
private:
    const char *ssid;
    const char *password;
    const char *serverAddress;
    int serverPort;

public:
    WiFiClientController(const char *ssid, const char *password, const char *serverAddress, int serverPort) {
        this->ssid = ssid;
        this->password = password;
        this->serverAddress = serverAddress;
        this->serverPort = serverPort;
    }

    bool connectToWiFi() {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected to WiFi");
        return true;
    }

    bool connectToServer() {
        WiFiClient client;
        if (!client.connect(serverAddress, serverPort)) {
            Serial.println("Connection Actuator to Server failed");
            return false;
        }
        return true;
    }

    String getRequest() {
        WiFiClient client;
        if (!client.connect(serverAddress, serverPort)) {
            Serial.println("Connection Actuator to Server failed");
            return "";
        }
        client.print("GET\n");
        delay(100);
        if (client.available()) {
            return client.readStringUntil('\n');
        } else {
            Serial.println("No response from server");
            return "";
        }
    }
};

const int ledPins[7] = {32, 33, 25, 26, 27, 14, 12};

void setup() {
    Serial.begin(115200);
    LEDController ledController(ledPins);
    WiFiClientController wifiController(WIFI_SSID, WIFI_PASS, SERVER_ADDRESS, SERVER_PORT);
    wifiController.connectToWiFi();
}

void loop() {
    WiFiClientController wifiController(WIFI_SSID, WIFI_PASS, SERVER_ADDRESS, SERVER_PORT);
    if (!wifiController.connectToServer()) {
        delay(1000);
        return;
    }
    String serverResp = wifiController.getRequest();
    if (serverResp.length() > 0) {
        bool ledStates[7];
        for (int i = 0; i < 7; i++) {
            ledStates[i] = serverResp[i] - '0';
        }
        LEDController ledController(ledPins);
        ledController.updateLEDs(ledStates);
    }
    Serial.println("Actuator Up and running");
    delay(100);
}

