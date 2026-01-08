#include <ArduinoBLE.h>

const int ledPin = 12;
const int d1SpeedPin = 0;
const int d1in1Pin = 1;
const int d1in2Pin = 2;
const int d2in3Pin = 3;
const int d2in4Pin = 4; 
const int d2SpeedPin = 5;
const int drSpeedPin = 6;
const int drin1Pin = 7;
const int drin2Pin = 8;
const int a1in3Pin = 9;
const int a1in4Pin = 10;
const int a1SpeedPin = 11;

// Custom UUIDs
BLEService ledService("26738915-1234-4321-1234-1a2b3c4d5e6f");
BLEUnsignedCharCharacteristic brightnessChar(
  "4d3c2b1a-4321-1234-4321-280980280980",
  BLEWrite | BLEWriteWithoutResponse | BLERead
);

BLEUnsignedCharCharacteristic d1Char(
  "4d3c2b1a-4321-1234-4321-d1d1d1d1d1d1",
  BLEWrite | BLEWriteWithoutResponse | BLERead
);
BLEUnsignedCharCharacteristic d2Char(
  "4d3c2b1a-4321-1234-4321-d2d2d2d2d2d2",
  BLEWrite | BLEWriteWithoutResponse | BLERead
);

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(d1SpeedPin,OUTPUT);
  pinMode(d1in1Pin,OUTPUT);
  pinMode(d1in2Pin,OUTPUT);
  pinMode(d2in3Pin,OUTPUT);
  pinMode(d2in4Pin,OUTPUT);
  pinMode(d2SpeedPin,OUTPUT);
  pinMode(drSpeedPin,OUTPUT);
  pinMode(drin1Pin, OUTPUT);
  pinMode(drin2Pin, OUTPUT);
  pinMode(a1in3Pin,OUTPUT);
  pinMode(a1in4Pin,OUTPUT);
  pinMode(a1SpeedPin,OUTPUT);

  if (!BLE.begin()) {
    while (1); // halt if BLE fails
  }

  BLE.setLocalName("Nano33BLE_LED");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(d1Char);
  ledService.addCharacteristic(d2Char);
  ledService.addCharacteristic(brightnessChar);
  BLE.addService(ledService);



  brightnessChar.writeValue(128); // initial brightness
  d1Char.writeValue(0);
  d2Char.writeValue(0);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {
      
      if (brightnessChar.written()) {
        // Always read the *current* value directly
        int brightness = brightnessChar.value();
        analogWrite(ledPin, brightness);
      }

      if (d1Char.written()) {
        // Always read the *current* value directly
        int speed = d1Char.value();
        if(speed>127){
          speed -= 127 ;
          speed *= 2;//math error probably
          digitalWrite(d1in1Pin,HIGH);
          digitalWrite(d1in2Pin,LOW);
        }else if(speed == 127){
          speed = 0;
        }else{
          speed = 255-speed;
          digitalWrite(d1in2Pin,HIGH);
          digitalWrite(d1in1Pin,LOW);
        }
        analogWrite(d1SpeedPin, speed);
      }

      if (d2Char.written()) {
        // Always read the *current* value directly
        int speed2 = d2Char.value();
        if(speed2>127){
          speed2 -= 127;
          speed2 *= 2;//math error probably
          digitalWrite(d2in3Pin,HIGH);
          digitalWrite(d2in4Pin,LOW);
        }else if(speed2 == 127){
          speed2 = 0;
        }else{
          speed2 = 255-speed2;
          digitalWrite(d2in4Pin,HIGH);
          digitalWrite(d2in3Pin,LOW);
        }
        analogWrite(d2SpeedPin, speed2);
      }





      delay(10); // keep connection stable
    }
  }
}
