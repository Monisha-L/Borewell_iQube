const float OffSet =  1.960;
float V, P;
void setup()
{
 Serial.begin(9600); // open serial port, set the baud rate to 9600 b
 Serial.println("/* Water pressure sensor demo */");
}
void loop()
{
 //Connect sensor to Analog 0
 V = analogRead(12) * 5.00 / 1024; //Sensor output voltage
 Serial.println(analogRead(12));
 P = (V - OffSet) * 400; //Calculate water pressure
 Serial.print("Voltage:");
 Serial.print(V, 7);
 Serial.println("V");
 Serial.print(" Pressure:");
 Serial.print(P, 10);
 Serial.println(" KPa");
 Serial.println();
 
 delay(500);
}
