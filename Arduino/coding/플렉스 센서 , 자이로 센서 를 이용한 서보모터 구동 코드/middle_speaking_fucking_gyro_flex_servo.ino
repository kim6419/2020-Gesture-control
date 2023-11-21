#include <Wire.h>
#include <Servo.h>

#define I2C_Address 0x53  
 
//ADXL345 register address
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define X_axis 0x32
#define Y_axis 0x34
#define Z_axis 0x36
 
#define Range_2g 0
#define Range_4g 1
#define Range_8g 2
#define Range_16g 3

int pinServoX = 9;
int pinServoY = 10;

int flexpin1 = A0;
int flexpin2 = A1;
int flexpin3 = A2;

Servo myServoX;
Servo myServoY;

void setup() {
  Wire.begin();                 // 센서 작동 시작
  Serial.begin(9600);           // 통신속도 9600으로 설정
  Init_ADXL345(Range_2g); 

  {
    myServoX.attach(pinServoX);
    myServoY.attach(pinServoY);
  delay(1500);
  }
}
 
void loop() {

  int flexVal1;
  int flexVal2;
  int flexVal3;

  flexVal1 = analogRead(flexpin1);
  flexVal2 = analogRead(flexpin2);
  flexVal3 = analogRead(flexpin3);

  /* 
   myServoX.write(Read_Axis(X_axis));
  myServoY.write(Read_Axis(Y_axis));
  */
  
  //3축 출력
  Serial.print("X: ");
  Serial.print(Read_Axis(X_axis));
  Serial.print("  Y: ");
  Serial.print(Read_Axis(Y_axis));
  Serial.print("  Z: ");
  Serial.print(Read_Axis(Z_axis));

  Serial.print("   플1: ");
  Serial.print(flexVal1);
  Serial.print("플2: ");
  Serial.print(flexVal2);
  Serial.print("플3: ");
  Serial.println(flexVal3);
  
  delay(100);

  if(flexVal1 >= 820 && flexVal2 >= 830 && flexVal3 < 920){
    myServoX.write(Read_Axis(X_axis));
  }if (flexVal1 >= 820 && flexVal2 >= 830 && flexVal3 >= 920){
    myServoY.write(Read_Axis(Y_axis));
  }
}
 
//I2C인터페이스를 통해 축을 읽음
int Read_Axis(byte a){
  int data;
 
  Wire.beginTransmission(I2C_Address); 
  Wire.write(a); 
  Wire.endTransmission(); 
 
  Wire.beginTransmission(I2C_Address); 
  Wire.requestFrom(I2C_Address, 2);
 
  if(Wire.available()){
    data=(int)Wire.read();
    data=data|(Wire.read()<<8);
  }
  else{
   data=0;
  }
 
  Wire.endTransmission();
  return data;
}
 
//ADXL345 초기화
void Init_ADXL345(byte r){
  
  Wire.beginTransmission(I2C_Address);
  
  //감도설정
  Wire.write(DATA_FORMAT);
  Wire.write(r);
  Wire.endTransmission();
 
  //측정모드로 전환
  Wire.beginTransmission(I2C_Address);
  Wire.write(POWER_CTL);
  Wire.write(0x08);
  Wire.endTransmission();
}
