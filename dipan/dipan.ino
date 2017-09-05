#include <AFMotor.h>
#define  hsensorR A0
#define  hsensorL A1
#define bsensorR A2
#define bsensorL A3
#define  MOVE A4
#define  pushing A5
#define MOVEBACK 2
int valR=0;
int valL=0;
int bvalR=0;
int bvalL=0;
int push=0;
AF_DCMotor motorl(4);//左轮
AF_DCMotor motorr(3);//右轮
AF_DCMotor motorp(2);//推杆
int moving=0;
int bmoveing=0;
void setup() {
  Serial.begin(9600); 
  motorl.setSpeed(0);
  motorr.setSpeed(0);
  motorp.setSpeed(0);
  motorl.run(RELEASE);
  motorr.run(RELEASE);
  motorp.run(RELEASE);
  pinMode(hsensorR,INPUT);
  pinMode(hsensorL,INPUT);
  pinMode(bsensorR,INPUT);
  pinMode(bsensorL,INPUT);
  pinMode(MOVE,INPUT);
  pinMode(pushing,INPUT);
  pinMode(MOVEBACK,INPUT);
  valR=digitalRead(hsensorR);
  valL=digitalRead(hsensorL);
  bvalR=digitalRead(bsensorR);
  bvalL=digitalRead(bsensorL);
}
void ahead()
{
  motorr.setSpeed(255);
  motorl.setSpeed(255);
  motorr.run(FORWARD);
  motorl.run(FORWARD);
  Serial.println("ahead");
}
void back()
{
  motorr.setSpeed(255);
  motorl.setSpeed(255);
  motorr.run(BACKWARD);
  motorl.run(BACKWARD);
}
void left()
{
  motorr.setSpeed(255);
  motorl.setSpeed(0);
  motorr.run(FORWARD);
  motorl.run(RELEASE);
  Serial.println("left");
 }
 void right()
 {
    motorr.setSpeed(0);
    motorl.setSpeed(255);
    motorr.run(RELEASE);
    motorl.run(FORWARD);
    Serial.println("right");
 }
 void bleft()
{
  motorr.setSpeed(255);
  motorl.setSpeed(0);
  motorr.run(BACKWARD);
  motorl.run(RELEASE);
  Serial.println("left");
 }
 void bright()
 {
    motorr.setSpeed(0);
    motorl.setSpeed(255);
    motorr.run(RELEASE);
    motorl.run(BACKWARD);
    Serial.println("right");
 }
void stoped()
{
     motorr.setSpeed(0);
     motorl.setSpeed(0);
    motorl.run(RELEASE);
    motorr.run(RELEASE);
    Serial.println("stop");
}
void Read()
{
  valR=digitalRead(hsensorR);
  valL=digitalRead(hsensorL);
  bvalR=digitalRead(bsensorR);
  bvalL=digitalRead(bsensorL);
  Serial.println("hava read");
 }
void followline()
{
  Read();
  if(valR==1)
  {
     while(!(valR==0&&valL==0))
      {
         right();
        Read();
   }
  }
  if(valL==1)
   { 
      while(!(valR==0&&valL==0))
      {
         left();
        Read();
       }
    }
    if(valR==0&&valL==0)
    {
      ahead(); 
    }
}
void bfollowline()
{
  Read();
  if(bvalR==1)
  {
     while(!(bvalR==0&&bvalL==0))
      {
        bleft();
        Read();
   }
  }
  if(bvalL==1)
   { 
      while(!(bvalR==0&&bvalL==0))
      {
        bright();
        Read();
       }
    }
    if(bvalR==0&&bvalL==0)
    {
      back(); 
    }
}
void pushed()
{
   motorp.setSpeed(255);
   motorp.run(FORWARD);
   delay(3000);
   motorp.setSpeed(255);
   motorp.run(BACKWARD);
   delay(3000);
   motorp.setSpeed(0);
   motorp.run(RELEASE);
}
void loop() {
  moving=digitalRead(MOVE);
  bmoveing=digitalRead(MOVEBACK);
  if(moving)
  {
      followline();
  }
  else
  {
    stoped();
    push=digitalRead(pushing) ;
    if(push)
    {
      pushed();
    }
  }
  while(bmoveing)
  {
    bmoveing=digitalRead(MOVEBACK);
    followline();
  }
}
