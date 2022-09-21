#include <WiFi.h>
#include <PubSubClient.h>

String apiKey = "22W59RQHFMVGNR19"; // API key from ThingSpeak
 
const char *ssid = "iQube"; 
const char *pass = "WeEvolve@iQube";
const char* server = "api.thingspeak.com";

const float  OffSet = 0.444;
float A, V, P;

WiFiClient client;
 
void setup()
{
Serial.begin(115200);
delay(10);
 
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
 
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}
 
void loop()
{
  A = analogRead(12);
  V = analogRead(12) * 5.00 / 1024;     
  P = (V - OffSet) * 400;             
  
  //char VoltString[8];
  Serial.print("Voltage:");
  Serial.print(V, 3);
  Serial.println("V");
 
  //char PressString[8];
  Serial.print(" Pressure:");
  Serial.print(P, 1);
  Serial.println(" KPa");
  Serial.println();

  float H = P / (997 * 9.81);
  Serial.print("Height:");
  Serial.print(H, 2);
  Serial.println("m");
  Serial.println();
 
if (client.connect(server,80)) //184.106.153.149 or api.thingspeak.com
{
String postStr = apiKey;
postStr +="&field1=";
postStr += String(V);
postStr +="&field2=";
postStr += String(P);
postStr += "&field3=";
postStr += String(H);
postStr += "\r\n\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: 22W59RQHFMVGNR19\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
Serial.println("Sent data to Thingspeak");
}
client.stop();
Serial.println("Delay of 15 Sec");
// thingspeak needs minimum 15 sec delay between updates
delay(5000);
}
