#include <SoftwareSerial.h>
#define iffinish 7
#define start 6
#define reload 5
SoftwareSerial myserial(10,11);
class Rfid{
private:
  HardwareSerial *ser;
  static uint8_t rfid_encode(uint16_t id, uint8_t *buf, uint8_t *msg, uint8_t msg_len){
    uint8_t *p, *tmp, check=0;
    p = buf;
    *p++ = 0xAA;
    *p++ = 0xBB;
    *p = msg_len + 3;
    if(*p==0xAA){
      *++p = 0;
    }
    p++;
    *p++ = 0;
    *p = id>>8;
    check ^= *p;
    if(*p==0xAA){
      *++p = 0;
    }
    p++;
    *p = id & 0x00ff;
    check ^= *p;
    if(*p==0xAA){
      *++p = 0;
    }
    p++;
    for(tmp=msg;tmp-msg<msg_len;tmp++,p++){
      *p = *tmp;
      check ^= *p;
      if(*p==0xAA){
        *p = 0;
      }
    }
    *p++ = check;
    return p-buf;
  }
 
  // 读取发来的字节数组，并按ISO15693协议解析出其有效载荷
  int rfid_recv(uint8_t *msg){
    int recv,cnt;
    for(cnt=0;cnt<9;cnt++){
      recv = ser->read();
      if(recv==-1){
        ser->flush();
        return -1;
      }
      if(recv==0xAA&&cnt!=0) ser->read();
    }
    for(cnt=0;(recv=ser->read())!=-1;cnt++){
      msg[cnt] = recv;
      if(recv==0xAA) ser->read();
    }
    ser->flush();
    return cnt-1;
  }
  void rfid_init(){
    int len;
    uint8_t buf[0x50], msg[] = {0x08,0x01,'1'};
    ser->begin(19200);
    len = rfid_encode(0,buf,msg,sizeof(msg));
    ser->write(buf,len);
    delay(200);
    rfid_recv(buf);
  }
public:
 
  // 构造函数
  Rfid(HardwareSerial &ser_in){
    ser = &ser_in;
    rfid_init();
  }
  // 读取电子标签的标签号，若标签不存在或失败返回-1，成功返回0，读到的9个字节放在data指针处
  int readID(uint8_t *data){
    int len;
    uint8_t msg[0x20] = {0x01,0x10}, buf[0x30];
     
    len = rfid_encode(0,buf,msg,2);
    ser->write(buf,len);
   
    delay(35);
    len = rfid_recv(data);
 
    if(len!=9){
      return -1;
    }else{
      return 0;
    }
  }
 
  // 读取电子标签的前4个字节，若标签不存在或失败返回-1，成功返回0，读到的4个字节放在data指针处
  int read4bytes(uint8_t *data){
    int len;
    uint8_t msg[0x20] = {0x01,0x10}, buf[0x30];
     
    len = rfid_encode(0,buf,msg,2);
    ser->write(buf,len);
   
    delay(35);
    len = rfid_recv(msg+2);
    if(len!=9){
      return -1;
    }else{    
      msg[0] = 0x05; msg[1] = 0x10; msg[11] = 0; msg[12] = 1;
      len = rfid_encode(0,buf,msg,13);
      ser->write(buf,len);
      delay(45);
      len = rfid_recv(msg);
      if(len!=4){
        return -1;
      }else{
        memcpy(data,msg,4);
        return 0;
      }
    }
  }
 void write4byte()
{
  int len;
    uint8_t msg[0x20] = {0x01,0x10}, buf[0x30];
     
    len = rfid_encode(0,buf,msg,2);
    ser->write(buf,len);
   
    delay(35);
      msg[0] = 0x06; msg[1] = 0x10; msg[11] = 0; msg[12] =0x05;msg[13]=0x00;msg[14]=0x00;msg[15]=0x00;
      len = rfid_encode(0,buf,msg,16);
      ser->write(buf,len);
  }
};

Rfid *r;
Rfid *r1;
int finished=0;
int saved=LOW;
String comdata = "";
uint8_t data[10][4],data1[4];
int present=0;
int error=0;
int count=0;
uint8_t flag=6;
void setup() {
  Serial.begin(9600);
  myserial.begin(19200);
  pinMode(9,INPUT); //是否储存完毕
  pinMode(8,OUTPUT);//车是否运动
  pinMode(iffinish,INPUT); //上部电机完成信号;7
  pinMode(start,OUTPUT);//6
  pinMode(reload,OUTPUT); //5
  digitalWrite(8,LOW);
  r = new Rfid(Serial1);
  r1=new Rfid(Serial2);
for(int i=0;i<10;)
  {
  if(r->read4bytes(data[i])==-1){
    Serial.println("No.1 no card");
  }else
   {
     if(i>0)
     {
      for(int j=i-1;j--;j>=0)
      {
        if((data[i][0]==data[j][0])&&(data[i][1]==data[j][1]))//去重
        {
        Serial.println("error");
        error=1;
        break;
        }
        }
      }
      if(error==0)
         i++;  
      else
      error=0;
      }
    }
    myserial.write(flag);
    sendinformation();
    saved=digitalRead(9);
    while(saved==LOW)
    {
      
      saved=digitalRead(9);
    }
    Serial.println("Saved");
    delay(3000);
}
void sendinformation()
{
  for(int i=0;i<10;i+=2)
  {
    for(int k=0;k<4;k++)
    {
      Serial.print(data[i][k]);
      Serial.print(' ');
    }
    Serial.println();
  }
  
  for(int i=0;i<10;i+=2)
  {
    for(int k=0;k<4;k++)
    {
      myserial.write(data[i][k]);
      delay(25);
    }
  }
  Serial.println("HAVE SEND");
} 

void loop() {
   Serial.println("Moving");
   digitalWrite(8,HIGH);
   
   if(r1->read4bytes(data1)==-1){
    Serial.println("No.2 no card");
   }
   else
   {
    Serial.println(data1[0]);
    if(data1[0]!=present)
    {
     for(int i=0;i<10;i+=2)
      {
         if(data[i][0]==data1[0]){
         Serial.println("Have fined") ;
         count++;
         present=data1[0];
         myserial.write(data1[0]);
         delay(25);
         Serial.println(data1[0]);
         digitalWrite(start,HIGH);
         digitalWrite(8,LOW);
         finished=digitalRead(iffinish);
         while(!finished)
         {
            finished=digitalRead(iffinish);
         }
         digitalWrite(start,LOW);
       }
      }
    }
   }
   if(count==5)
   {
      digitalWrite(8,LOW);
      digitalWrite(reload,HIGH);
      delay(3000);
      digitalWrite(reload,LOW);
      count=0;
      for(int i=0;i<10;)
      {
      if(r->read4bytes(data[i])==-1){
        Serial.println("No.1 no card");
      }else
       {
         if(i>0)
         {
          for(int j=i-1;j--;j>=0)
          {
            if((data[i][0]==data[j][0])&&(data[i][1]==data[j][1]))//去重
            {
            Serial.println("error");
            error=1;
            break;
            }
            }
          }
          if(error==0)
             i++;  
          else
          error=0;
          }
        }
        myserial.write(flag);
        sendinformation();
        saved=digitalRead(9);
        while(saved==LOW)
        {
          
          saved=digitalRead(9);
        }
        Serial.println("Saved");
        delay(3000);
       }
       
}

