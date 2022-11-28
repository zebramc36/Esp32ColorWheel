#include "BluetoothSerial.h"
// using library analogwrite - ERROPiX v1.0
#include <analogWrite.h>
#define redPin 12    
#define greenPin 13  
#define bluePin 14   
#define comPin 27

BluetoothSerial SerialBT;

int ledPin = 2;
String data;
String data_bt;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  SerialBT.begin("ESP32_kachun"); //Bluetooth device name
  pinMode(redPin, OUTPUT);   
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT);  
  pinMode(comPin, OUTPUT);    
  Serial.println("The device started, now you can pair it with bluetooth!");
  data = "";
  data_bt = "";
}

void loop() {
  while (Serial.available()) {
    delay(5);
    char c = Serial.read();
    data += c;
  }
  String currData = "";
  while (SerialBT.available()) {
    //delay(5);
    //currData = SerialBT.readString();
    char c = SerialBT.read();
    currData += c;        
    if (currData.length() == 9) {
      break;
    }
  }  
  // only 9-digit i.e. color channel is accepted
  if (currData.length() == 9) {
    if (currData != data_bt) {
      data_bt = currData;
      Serial.println("BT Received: " + data_bt);
        analogWrite(comPin, 255);
        int r = data_bt.toInt() / 1000000;
        int g = data_bt.toInt() / 1000 - r * 1000;
        int b = data_bt.toInt() - r * 1000000 - g * 1000;
        Serial.println("Red: " + String(r));
        Serial.println("Green: " + String(g));
        Serial.println("Blue: " + String(b));
        analogWrite(redPin, (255-r));        
        analogWrite(greenPin, (255-g));
        analogWrite(bluePin, (255-b));        
    }    
  }
  //currData = "";
  if (data.length() > 0) {
    Serial.println(data);

    if (data == "on\n") {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }
    if (data == "off\n") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
    data = "";
  }
  delay(20);
}