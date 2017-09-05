#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50
SoftwareSerial mySerial(10,11);
uint8_t  point=0;
int local=3;
int finded=0;
int count=0;
uint8_t data[5][4];
uint8_t flag=0;
#define start 6 //开始动作 输入
#define pushed 8 //推杆运动 输出
#define saved 9 //保存图书信息 输出
#define finish 7 //完成动作 输出
void setup() {
  Serial.begin(9600);
  mySerial.begin(19200);    
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(start,INPUT);
  pinMode(pushed,OUTPUT);
  pinMode(saved,OUTPUT);
  pinMode(finish,OUTPUT);
  digitalWrite(A3,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(8,LOW);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pwm.setPWM(0, 0, pulseWidth(208));
  pwm.setPWM(3,0,pulseWidth(-20));
  pwm.setPWM(2, 0, pulseWidth(80));
  digitalWrite(saved,LOW);
  while(!data[0][0])
 {
   delay(1000);
   bookinformation();
   delay(1000);
  }
  Serial.println("Saved");
  digitalWrite(saved,HIGH);
}
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}
void bookinformation()
{
  if (mySerial.available()>0){
    flag=mySerial.read();
    if(flag==6)
    {
       for(int i=0;i<5;i++)
      {
        for(int k=0;k<4;k++)
        {
          data[i][k]=mySerial.read();
          delay(25);
        }
      }
    }
  }
 else
     Serial.println("error");
for(int i=0;i<5;i++)
  {
    for(int k=0;k<4;k++)
    {
      Serial.print(data[i][k]);
      Serial.print(' ');
    }
    Serial.println();
  }

}
 void stepper2(int8_t f)
 {
    if(f>0)
    digitalWrite(A3,LOW);
    else
    {
      f=0-f;
    digitalWrite(A3,HIGH);
    }
   for(int i=f*2000;i>0;i--)
  {
    digitalWrite(A2,HIGH);
  delayMicroseconds(400); 
    digitalWrite(A2,LOW);
  delayMicroseconds(400);
  }
  }
  void stepper1(int8_t f)
 {
    if(f>0)
    digitalWrite(A1,HIGH);
    else
    {
      f=0-f;
    digitalWrite(A1,LOW);
    }
   for(int i=(f+1)*1400;i>0;i--)
  {
    digitalWrite(A0,HIGH);
  delayMicroseconds(400); 
    digitalWrite(A0,LOW);
  delayMicroseconds(400);
  }
  }
  void clean()
  {
      for(int i=0;i<5;i++) 
        for(int j=0;j<4;j++)
        {
         data[i][j]=0 ;
        }
   }
void loop() {
  uint8_t temp;
  finded=digitalRead(start);
  digitalWrite(finish,LOW);
 if(finded)
 {
   if(mySerial.available()){
    while((!temp)||(temp>5))
      temp=mySerial.read();
      point=temp;
      temp=0;
    }
   Serial.println(point);
   for(uint8_t i=0;i<5;i++)
   {
      if(data[i][0]==point)
      {
          Serial.print("stepper2 start");
          stepper2((i+1)-local);
          local=(i+1);
          Serial.print("push start");
          digitalWrite(pushed,HIGH);
          delay(3000);
          pwm.setPWM(2, 0, pulseWidth(130));
          digitalWrite(pushed,LOW);
          Serial.print("stepper1 start");
          stepper1(data[i][1]-0);
          Serial.print("servo start");
          for(int i=208;i>=90;i--)
          {
          pwm.setPWM(0, 0, pulseWidth(i));
          }
          for(int i=-20;i<=90;i++)
          {
          pwm.setPWM(3, 0, pulseWidth(i));
          }
          delay(3000);
          pwm.setPWM(2, 0, pulseWidth(80));
          //放书
          delay(3000);
          for(int i=90;i<=208;i++)
          {
          pwm.setPWM(0, 0, pulseWidth(i));
          }
          for(int i=90;i>=-20;i--)
          {
          pwm.setPWM(3, 0, pulseWidth(i));
          }
          stepper1(0-data[i][1]);
          //恢复
          digitalWrite(finish,HIGH);
          count++;
          delay(100);
      }
    }
  }
  if(count==5)
  {
    stepper2(3-local);
    local=3;
    clean();
    digitalWrite(saved,LOW);
  while(!data[0][0])
 {
   delay(1000);
   bookinformation();
   delay(1000);
  }
  Serial.println("Saved");
  digitalWrite(saved,HIGH);
  count=0;
  }
}
