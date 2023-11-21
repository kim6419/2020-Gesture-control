#include <Wire.h>
#include <Mouse.h>
#include <Keyboard.h>

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

int flexpin1 = A0;
int flexpin2 = A1;
int flexpin3 = A2;
int flexpin4 = A3;

const int MouseXAxis = A4;
const int MouseYAxis = A5;
int selPin = 9;
int mouseClickFlag = 0;
int invertMouse = -1;

void setup(){
  Wire.begin();
  Serial.begin(9600);
  Init_ADXL345(Range_2g);
  pinMode(10,INPUT);
  pinMode(selPin,INPUT);
  digitalWrite(selPin,HIGH);
  delay(1000);
  Mouse.begin();
  Keyboard.begin();
  
} 
void loop(){
  int flexVal1;
  int flexVal2;
  int flexVal3;
  int flexVal4;

  int x = readAxis(MouseXAxis);
  int y = readAxis(MouseYAxis);

  flexVal1 = analogRead(flexpin1);
  flexVal2 = analogRead(flexpin2);
  flexVal3 = analogRead(flexpin3);
  flexVal4 = analogRead(flexpin4);

  Mouse.move(x,y,0);
  delay(5);

 { Serial.print("X: ");
  Serial.print(Read_Axis(X_axis));
  Serial.print("  Y: ");
  Serial.print(Read_Axis(Y_axis));
  Serial.print("  Z: ");
  Serial.print(Read_Axis(Z_axis));
  Serial.print(" ");
  
  Serial.print("플1:");
  Serial.print(flexVal1);
  Serial.print(" ");
  Serial.print("플2:");
  Serial.print(flexVal2);
  Serial.print(" ");
  Serial.print("플3:");
  Serial.print(flexVal3);
  Serial.print(" ");
  Serial.print("플4:");
  Serial.print(flexVal4);
  Serial.print(" ");

  Serial.print("마우스x:");
  Serial.print(x);
  Serial.print("마우스y:");
  Serial.println(y);
 } 
  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
  }

  /* if(Read_Axis(Y_axis) > 120){
    Keyboard.press('A');
    delay(500);
  }if(Read_Axis(Y_axis) < -190){
    Keyboard.press('D');
    delay(500);
  }if(-189 <= Read_Axis(Y_axis) <= 120){
    Keyboard.releaseAll(); 
  }
   if(flexVal1 > 800){  
    Keyboard.press('W');  //전진
    delay(200);
   } if (flexVal1 <= 800){
    Keyboard.releaseAll();
   }if(flexVal2 > 820){
    Keyboard.write('F');        //드리프트
   }if(flexVal2 <= 820){
    Keyboard.releaseAll();
   } */

  if(flexVal3 > 820 && flexVal4 > 920){  // 4,5번 손가락을 굽혔을 때
    if(Read_Axis(Y_axis)>120){
      Keyboard.press('A');            //가속도 센서를 굽혔을 때 출력
      delay(500);
    }if(Read_Axis(Y_axis) < -190){  
      Keyboard.press('D');            // 가속도 센서를 굽혔을 때 출력
      delay(500);
    }if(-189 <= Read_Axis(Y_axis) <= 120){    // 조건에서 벗어나면
      Keyboard.releaseAll();
    }  
  }else{
    Keyboard.releaseAll();
  }

   if(flexVal1 > 800 && flexVal2 > 820 && flexVal3 > 820 && flexVal4 > 920){ //손 전체를 구부렸을때
    /* analogWrite(11,200);
    delay(300);
    analogWrite(11,200);
    delay(300);
    analogWrite(11,0);
    delay(3000000); */
    Mouse.move((Read_Axis(X_axis)/5*(-1)), Read_Axis(Y_axis)/5 , 0);    // 마우스를 가속도로 제어
    delay(5);
   }else{
    Mouse.release();
   }
  
}
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
int readAxis(int thisAxis){
  int reading = analogRead(thisAxis);
 
  reading = map(reading, 0, 1023, 0, 12);
 
  int distance = reading - 6;
 
  if (abs(distance) < 2) {
    distance = 0;
  }
  return distance;
}
