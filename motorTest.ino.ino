
int ledPin = 12;
int d1SpeedPin = 0;
int d1in1Pin = 1;
int d1in2Pin = 2;
int d2in3Pin = 3;
int d2in4Pin = 4; 
int d2SpeedPin = 5;
int drSpeedPin = 6;
int drin1Pin = 7;
int drin2Pin = 8;
int a1in3Pin = 9;
int a1in4Pin = 10;
int a1SpeedPin = 11;





void setup() {
  // put your setup code here, to run once:
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


  delay(1000);
  digitalWrite(ledPin,HIGH);
  delay(1000);
  digitalWrite(ledPin,LOW);
  delay(1000);
  analogWrite(ledPin,127);
  delay(1000);
  digitalWrite(ledPin,LOW);
  delay(1000);

while(1){
  digitalWrite(d1in2Pin,LOW);
  digitalWrite(d1in1Pin,HIGH);
  analogWrite(d1SpeedPin,255);
  delay(1000);
  analogWrite(d1SpeedPin,127);
  delay(1000);
  analogWrite(d1SpeedPin,255);
  digitalWrite(d1in1Pin,LOW);
  digitalWrite(d1in2Pin,HIGH);
  delay(1000);
}

  // analogWrite(d1SpeedPin,0);
  // analogWrite(d2SpeedPin,255);
  // digitalWrite(d2in3Pin,HIGH);
  // delay(1000);
  // digitalWrite(d2in3Pin,LOW);
  // digitalWrite(d2in4Pin,HIGH);
  // analogWrite(d2SpeedPin,127);
  // delay(1000);
  // analogWrite(d2SpeedPin,0);  
  
  // analogWrite(drSpeedPin,255);
  // digitalWrite(drin1Pin,HIGH);
  // delay(1000);
  // digitalWrite(drin1Pin,LOW);
  // digitalWrite(drin2Pin,HIGH);
  // analogWrite(drSpeedPin,127);
  // delay(1000);
  // analogWrite(drSpeedPin,0);  
  
  // analogWrite(a1SpeedPin,255);
  // digitalWrite(a1in3Pin,HIGH);
  // delay(1000);
  // digitalWrite(a1in3Pin,LOW);
  // digitalWrite(a1in4Pin,HIGH);
  // analogWrite(a1SpeedPin,127);
  // delay(1000);
  // analogWrite(a1SpeedPin,0);  


  




  

}

void loop() {
  // put your main code here, to run repeatedly:

}
