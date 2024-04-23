#include <Servo.h>
#include "DHT.h"
#include <SoftwareSerial.h>

#define DHTPIN1 2    
#define DHTPIN2 3     
#define DHTTYPE DHT11   // DHT 11 

SoftwareSerial BTSerial(2, 3);

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

Servo myServo;

const int relayPins[] = {13,12,11};//0,electricity - 1,waterPump - 2,airFan .
const int smkPin= A0; 
const int flmPin = 4;
const int servoPin =5;
void setup() {
 pinMode(flmPin,INPUT);
 pinMode(9,OUTPUT);
 pinMode(8,OUTPUT);
 pinMode(10,OUTPUT);
 pinMode(6,OUTPUT);
 myServo.attach(servoPin);
  for (int i = 0; i < 3; i++) {
    pinMode(relayPins[i], OUTPUT);
    Serial.begin(9600);
     dht1.begin();
  dht2.begin();
  }

 BTSerial.begin(9600);  // Bluetooth module
 
}

void loop() {
  int smkValue = map(analogRead(smkPin),0,1024,0,255);
  int flmValue= digitalRead(flmPin); 
 
delay(100);
  
  digitalWrite(9,1);
   for (int angle = 0; angle <= 180; angle += 1) {
if (flmValue ==0){
  
  digitalWrite(relayPins[0],0 );
  digitalWrite(relayPins[1],1);
   Serial.println("flame detected");
   delay(100);
  }
else {
  
  Serial.println("No flame");
  delay(100);
   digitalWrite(relayPins[0],1);
  digitalWrite(relayPins[1],0);
  }
  if (smkValue >= 30 ){
  digitalWrite(relayPins[2],1);
  Serial.println(smkValue);
  }
  else {  
 digitalWrite(relayPins[2],0);
 Serial.println(smkValue);
  }
  

    myServo.write(angle);
    delay(15);
  
  
  float humidity1 = dht1.readHumidity();
  float temperature1 = dht1.readTemperature();

  float humidity2 = dht2.readHumidity();
  float temperature2 = dht2.readTemperature();

  if (isnan(humidity1) || isnan(temperature1) || isnan(humidity2) || isnan(temperature2)) {
    Serial.println("Failed to read from one or both DHT sensors!");
    return;
  }
  delay(500);
  Serial.print("Sensor 1 - Humidity: ");
  Serial.print(humidity1);
  Serial.print("%  Temperature: ");
  Serial.print(temperature1);
  Serial.println("°C ");

  Serial.print("Sensor 2 - Humidity: ");
  Serial.print(humidity2);
  Serial.print("%  Temperature: ");
  Serial.print(temperature2);
  Serial.println("°C ");
  Serial.print("The result :");
  Serial.println(temperature1-temperature2);
  
   }

   if (BTSerial.available()) {
    Serial.println("Connected");
    String receivedString = "";
    char receivedChar;
    
    while (BTSerial.available() && (receivedChar = BTSerial.read()) != '\n') {
      receivedString += receivedChar;
      delay(2);
    }
    
    Serial.print("Received: ");
    Serial.println(receivedString);
  }

  if (Serial.available()) {
    
    String input = Serial.readStringUntil('\n'); // Read until newline
    
    sendMessage(input);
  }
    for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15);  
  }
 
 }

 void sendMessage(String message) {
  BTSerial.print("N:"); // Prefix for notification
  BTSerial.println(message);
  Serial.print("Sent: ");
  Serial.println(message);
}
