#include <SoftwareSerial.h>        // 블루투스 시리얼 통신 라이브러리 추가
#include <Servo.h> 
#define BT_RXD 8
#define BT_TXD 7
int servoPin = 6;
SoftwareSerial bluetooth(BT_RXD, BT_TXD);        
String myString="";
Servo servo; 

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);                         // 블루투스 통신 시작
  servo.attach(servoPin);
}
void loop() {
  if (bluetooth.available()) {        // 블루투스에서 보낸 내용은 시리얼모니터로 전송
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {           // 시리얼모니터에서 보낸 내용은 블루투스로 전송
    bluetooth.write(Serial.read());
  }

while(bluetooth.available())  //mySerial 값이 있으면
  {
    char myChar = (char)bluetooth.read();  //bluetooth int형식의 값을 char형식으로 변환
    myString+=myChar;   //수신되는 문자열을 myString에 모두 붙임 (1바이트씩 전송되는 것을 모두 붙임)
    delay(50);           //수신 문자열 끊김 방지
  }
  
  if(!myString.equals(""))  //myString 값이 있다면
  {
  if(myString=="90")  //다른 기기에서 입력한 값이 90이면
      {
      Serial.println("입력값 : " +myString);
     servo.write(90);
     myString="";
      }
      
    else if(myString=="180")  //다른 기기에서 연결한 값이 180이면
      {
      Serial.println("입력값 : " +myString);
     servo.write(180);
     myString="";
      }
       
     else if(myString=="0")  //다른 기기에서 연결한 값이 0이면
      {
      Serial.println("입력값 : "+myString);
     servo.write(0);
     myString="";
      }
    
    myString="";
  }

}


  
