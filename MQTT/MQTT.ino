#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Router"; // Enter your WiFi name
const char *password = "123456789";  // Enter WiFi password

const char *mqtt_broker = "10.1.75.125";
const char *topic = "esp32/test";
const char *mqtt_username = "iqube";
const char *mqtt_password = "iQube@2019";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const float  OffSet = 0.444;
float A, V, P;

void setup()
{
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.println("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  // publish and subscribe
  client.publish(topic, "Data received");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop()
{
  client.loop();

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
}
