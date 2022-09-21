#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 


const float  OffSet = 0.444;
float A, V, P;

const char* ssid = "iQube";
const char* password = "WeEvolve@iQube";
const char *mqtt_username = "iqube";
const char *mqtt_password = "iQube@2019";
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;


const float  OffSet = 0.444;
float A, V, P;

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-2";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("borewell/comm");
      }      
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    } 
    Serial.println("");
     Serial.println("Connected to Wi-Fi");
  setupMQTT();
}


void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  long now = millis();
  long previous_time = 0;
  
  if (now - previous_time > 1000) {

    //Connect sensor to Analog 0
  A = analogRead(12);
  V = analogRead(12) * 5.00 / 1024;     //Sensor output voltage
  P = (V - OffSet) * 250;             //Calculate water pressure

  Serial.print("Analog pin:");
  Serial.println(A);


  Serial.print("Voltage:");
  Serial.print(V, 3);
  Serial.println("V");

  Serial.print(" Pressure:");
  Serial.print(P, 1);
  Serial.println(" KPa");
  Serial.println();

  delay(500);

    previous_time = now;

    
    
  }
}
  void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}
