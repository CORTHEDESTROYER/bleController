#include <ArduinoBLE.h>

const int batteryCheckPin = A7;
const int batteryCheckEnablePin = 20;
const int ledPin = 12;
bool ledState = false;
const float R1 = 220000.0; // top resistor 
const float R2 = 68000.0; // bottom resistor 
const float ADC_REF = 3.3;
const int ADC_MAX = 4095;

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

unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 2000; // read every 2 seconds
// --- State machine variables --- //
enum ReadState { IDLE, SETTLING, SAMPLING };
ReadState readState = IDLE;

unsigned long settleStart = 0; 
const unsigned long SETTLE_TIME = 100;
// ms for RC filter
const int NUM_SAMPLES = 10;
int sampleCount = 0; 
long sampleSum = 0; 
float batteryVoltage = 0.0;


// Custom UUIDs
BLEService MOTORCAR("26738915-1234-4321-1234-1a2b3c4d5e6f");

BLEUnsignedCharCharacteristic d1Char(
  "4d3c2b1a-4321-1234-4321-d1d1d1d1d1d1",
  BLEWrite | BLEWriteWithoutResponse
);
BLEUnsignedCharCharacteristic d2Char(
  "4d3c2b1a-4321-1234-4321-d2d2d2d2d2d2",
  BLEWrite | BLEWriteWithoutResponse 
);
BLEUnsignedCharCharacteristic drChar(
  "4d3c2b1a-4321-1234-4321-d0d0d0d0d0d0",
  BLEWrite | BLEWriteWithoutResponse 
);
BLEUnsignedCharCharacteristic a1Char(
  "4d3c2b1a-4321-1234-4321-a1a1a1a1a1a1",
  BLEWrite | BLEWriteWithoutResponse 
);
BLEUnsignedCharCharacteristic bChar(
  "4d3c2b1a-4321-1234-4321-babababababa",
  BLERead | BLENotify
);






void setup() {
  // Serial.begin(9600);
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

  pinMode(batteryCheckPin,INPUT);
  pinMode(batteryCheckEnablePin,OUTPUT);



  if (!BLE.begin()) {
    while (1); // halt if BLE fails
  }

  BLE.setLocalName("CDJST Motor Car");
  BLE.setAdvertisedService(MOTORCAR);

  MOTORCAR.addCharacteristic(d1Char);
  MOTORCAR.addCharacteristic(d2Char);
  MOTORCAR.addCharacteristic(drChar);
  MOTORCAR.addCharacteristic(a1Char);
  MOTORCAR.addCharacteristic(bChar);

  BLE.addService(MOTORCAR);

  d1Char.writeValue(0);
  d2Char.writeValue(0);
  drChar.writeValue(0);
  a1Char.writeValue(0);
  bChar.writeValue(0);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {
      if(ledState){digitalWrite(ledPin,HIGH);}
      unsigned long now = millis();
      

      if (d1Char.written()) {
        int speed = d1Char.value();
        if(speed>127){
          speed = map(speed,127,255,0,255);
          digitalWrite(d1in1Pin,HIGH);
          digitalWrite(d1in2Pin,LOW);
        }else if(speed == 127){
          speed = 0;
        }else{
          speed = map(speed,127,0,0,255);
          digitalWrite(d1in2Pin,HIGH);
          digitalWrite(d1in1Pin,LOW);
        }
        analogWrite(d1SpeedPin, speed);
      }

      if (d2Char.written()) {
        int speed2 = d2Char.value();
        if(speed2>127){
          speed2 = map(speed2,127,255,0,255);
          digitalWrite(d2in3Pin,HIGH);
          digitalWrite(d2in4Pin,LOW);
        }else if(speed2 == 127){
          speed2 = 0;
        }else{
          speed2 = map(speed2,127,0,0,255);
          digitalWrite(d2in4Pin,HIGH);
          digitalWrite(d2in3Pin,LOW);
        }
        analogWrite(d2SpeedPin, speed2);
      }

      if (drChar.written()) {
        int speed3 = drChar.value();
        if(speed3>127){
          speed3 = map(speed3,127,255,0,255);
          digitalWrite(drin1Pin,HIGH);
          digitalWrite(drin2Pin,LOW);
        }else if(speed3 == 127){
          speed3 = 0;
        }else{
          speed3 = map(speed3,127,0,0,255);

          digitalWrite(drin2Pin,HIGH);
          digitalWrite(drin1Pin,LOW);
        }
        analogWrite(drSpeedPin, speed3);
      }
      if (a1Char.written()) {
        int speed4 = a1Char.value();
        digitalWrite(a1in3Pin,HIGH);
        analogWrite(a1SpeedPin, speed4);
      }
      
      switch (readState) {
        case IDLE: 
          if (now - lastReadTime >= READ_INTERVAL) {
            lastReadTime = now;
            digitalWrite(batteryCheckEnablePin, HIGH);// turn divider on 
            settleStart = now;
            readState = SETTLING;
          } 
          break;
          case SETTLING:
            if (now - settleStart >= SETTLE_TIME) {
              sampleSum = 0;
              sampleCount = 0;
              readState = SAMPLING;
            } 
            break;
          case SAMPLING: 
            sampleSum += analogRead(batteryCheckPin);
            sampleCount++;
            if (sampleCount >= NUM_SAMPLES) {
              // digitalWrite(batteryCheckEnablePin, LOW); // turn divider off
              // Serial.println(sampleSum);
              float adc = sampleSum / float(NUM_SAMPLES);
              // Serial.println(adc);
              float vPin = (adc / ADC_MAX) * ADC_REF;
              // Serial.println(vPin);
              batteryVoltage = vPin * ((R1 + R2) / R2)*((R1 + R2) / R2); 
              // Serial.println(batteryVoltage);
              bChar.writeValue(batteryVoltage);

              if(batteryVoltage<8){
                ledState = true;
              }

            // int percent = map(batteryVoltage * 100, 960, 1150, 0, 100);


            // bChar.writeValue(percent);
              readState = IDLE; 
            }
            break; 

          }
      delay(10); // keep connection stable
    }
  }
}
