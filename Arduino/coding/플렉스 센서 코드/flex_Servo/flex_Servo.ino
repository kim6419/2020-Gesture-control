#include <Servo.h>

const int pinServo = 9;
const int pinFlex = A0;

Servo myServo;

void setup(){
  myServo.attach(pinServo);
}

void loop(){
  int flexValue;
  flexValue = analogRead(pinFlex);
  flexValue = map(flexValue, 180, 290, 0, 180);
  myServo.write(flexValue);
  delay(10);
}
