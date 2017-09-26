#include <Servo.h>  
Servo myservoA;  
Servo myservoB;  
Servo myservoC;  
int sea,seb,sec;
int i=20;
int i1=500;                  //运作之间的间隔时间
int pos = 20;   
int sudu=20;      //机械臂运行速度调整
int commaPosition;
int commaPosition1;
int commaPosition2;  
int commaPosition3;
int commaPosition4;
int commaPosition31;
int commaPosition41;
int commaPosition31qz;
int commaPosition41qz;
int commaPositiondl;

int qpy;
int qpx;
int qzi=0;

int xzhou=100;
int yzhou=150;
int zzhou=0;



String mycommand="";  //捕捉串口发来的坐标指令


void qibangon()   //气泵开
{
digitalWrite(13,HIGH);
digitalWrite(12,LOW);


}

void qibangoff() //气泵关
{
digitalWrite(13,LOW);
digitalWrite(12,HIGH);
}

const char* zb[9][9]  = {
/*
{"32,90,137","42,93,147","54,94,157","70,94,168","96,99,165","117,92,168","133,94,157","147,92,147","155,87,137"},
{"42,82,130","50,87,138","62,91,145","77,94,148","95,99,145","112,94,148","126,88,145","138,89,135","147,87,128"},
{"49,77,123","58,81,128","68,84,133","80,85,137","94,89,135","109,85,136","121,83,133","131,81,127","140,77,120"},
{"55,68,113","63,73,118","72,77,123","82,78,125","94,79,127","106,77,126","117,76,123","126,74,117","135,68,112"},
{"59,60,101","66,65,107","75,67,112","84,69,114","94,71,114","105,70,112","114,68,111","123,64,106","130,58,107"},
{"64,50,95","71,54,102","79,59,104","87,60,106","96,61,107","105,60,106","114,58,104","121,54,101","127,50,95"},
{"67,41,82","73,42,88","80,46,93","88,49,93","95,50,93","104,48,95","111,46,93","119,42,88","123,39,80"},
{"68,24,63","69,26,66","81,34,77","89,36,80","95,38,79","103,36,80","109,34,77","117,32,69","120,25,65"},
{"69,5,39","72,10,47","81,17,52","89,22,61","95,22,63","100,22,61","106,20,60","112,16,54","118,10,47"},
*/
{"29,85,134","38,87,146","50,89,155","65,89,162","85,94,170","109,89,162","128,89,155","142,87,146","152,85,134"},
{"39,80,126","48,85,134","59,86,143","71,89,147","87,94,143","106,89,147","121,86,142","133,85,134","143,80,126"},
{"46,72,119","54,78,123","64,81,129","75,82,133","88,85,133","103,82,134","116,81,129","126,78,123","135,72,119"},

{"51,68,106","59,72,112","67,76,116","77,75,122","88,77,121","101,75,122","113,76,116","121,72,111","129,67,106"},
{"57,60,96","63,65,101","70,68,105","80,69,108","90,70,109","100,69,107","109,68,105","118,65,100","126,60,96"},
{"60,49,86","66,55,90","73,58,94","81,61,94","90,62,95","99,61,94","107,58,94","115,55,90","122,49,86"},
{"63,39,73","68,45,77","75,46,81","82,49,83","90,50,83","98,49,83","105,46,81","112,43,77","119,39,73"},
{"65,25,59","70,30,63","76,35,67","82,36,68","90,36,69","97,36,68","104,35,67","110,30,63","116,25,59"},
{"66,7,38","71,15,43","77,20,50","83,20,51","90,22,52","96,23,51","103,20,50","108,15,43","114,7,36"},
};


const char* qh[]  = {
//  "49,75,93","55,66,83","60,55,73","64,43,58","68,28,42",   
   "37,69,108","43,60,98","48,50,88"," 53,39,75","57,24,59",
                     };    //第一行





void setup() 
{   
  Serial.begin(9600,SERIAL_8N1);
  myservoA.attach(9);  // 控制腰部（A）的端口是~3号
  myservoB.attach(10);  // 控制腰部（A）的端口是~3号
  myservoC.attach(11);  // 控制腰部（A）的端口是~3号

pinMode(12,OUTPUT);    //定义脚位  用于气泵
pinMode(13,OUTPUT);



}


//放子程序     
void runt(int A1,int A2,int A3)      //机械臂默认位置移到棋盘指定位置放子  
{

  qibangon();    //动作完成后 开启气泵
 for(pos = 0; pos <=sudu; pos += 1) 
{                                
    myservoA.write(map(pos,1,sudu,xzhou,A1));// 让大臂（A）从109度旋转到指定位置   109 91 90是机械臂的初始位置
    delay(i);                      

     myservoB.write(map(pos,1,sudu,yzhou,A2+15));// 让大臂（B）从91度旋转到124位置
                     
   delay(i);   


       myservoC.write(map(pos,1,sudu,zzhou,A3-15));// 让大臂（B）从91度旋转到124位置
                     
   delay(i);   

 
  }   
for(pos = 0; pos <=sudu; pos += 1) 
 {   
  myservoC.write(map(pos,1,sudu,A3-15,A3)); //让小臂(C) 从90度旋转到指定位置
   delay(i);  

     
   myservoB.write(map(pos,1,sudu,A2+15,A2));// 让大臂（B）从124度旋转到指定位置
    delay(i);    
    
                 
 }

             delay(20); 
 //   delay(100);   
   
     qibangoff();    //动作完成后  关闭气泵     
        delay(100);      
 for(pos = 0; pos <=sudu; pos += 1) 
  {            
 
    
    myservoB.write(map(pos,1,sudu,A2,A2+15));// 让大臂（B）从91度旋转到指定位置
                     
    delay(i);    

        myservoC.write(map(pos,1,sudu,A3,A3-15)); //让小臂(C) 从指定位置旋转到90度     使机械臂回到初始位置  
      delay(i);        
   }   
  for(pos = 0; pos <=sudu; pos += 1) 
  {       
    myservoB.write(map(pos,1,sudu,A2+15,yzhou));// 让大臂（B）从指定位置旋转到91度 
    delay(i);    
    
    myservoC.write(map(pos,1,sudu,A3-15,zzhou)); //让小臂(C) 从指定位置旋转到90度     使机械臂回到初始位置  
      delay(i);        
                 
  }   
 for(pos = 0; pos <=sudu; pos += 1) 
  {   
    myservoA.write(map(pos,1,sudu,A1,xzhou));// 让大臂（A）从指定位置旋转到109度 
    delay(i);                      
  }    
      delay(100);     
Serial.println("CCC1");

}



 
void runq(int A1,int A2,int A3)    //从棋盒取子动作
{
     qibangoff();    //动作完成后  关闭气泵  
 for(pos = 0; pos <=sudu; pos += 1) 
  {                                
    myservoA.write(map(pos,1,sudu,xzhou,A1));     // 让大臂（A）从109度旋转到指定位置   109 91 90是机械臂的初始位置
     delay(i);                        
     
     myservoC.write(map(pos,1,sudu,zzhou,A3-15));     //让小臂(C) 从90度旋转到指定位置
    delay(i);     
    myservoB.write(map(pos,1,sudu,yzhou,A2+15));     // 让大臂（B）从91度旋转到指定位置
        delay(i);  
      }
for(pos = 0; pos <=sudu; pos += 1) 
  {                         
     myservoC.write(map(pos,1,sudu,A3-15,A3));     //让小臂(C) 从90度旋转到指定位置
    delay(i);           
    myservoB.write(map(pos,1,sudu,A2+15,A2));   // 让大臂（B）从91度旋转到指定位置
                     
    delay(i);     
                    
  }
  qibangon();    //动作完成后 开启气泵
  
    delay(500);    
for(pos = 0; pos <=sudu; pos += 1) 
  {   
   myservoB.write(map(pos,1,sudu,A2,A2+15));// 让大臂（B）从91度旋转到指定位置
                     
    delay(i);    
    myservoC.write(map(pos,1,sudu,A3,A3-15)); //让小臂(C) 从指定位置旋转到90度     使机械臂回到初始位置    
    delay(i);    
   } 
  for(pos = 0; pos <=sudu; pos += 1) 
  {      
    myservoB.write(map(pos,1,sudu,A2+15,yzhou));// 让大臂（B）从指定位置旋转到91度 
    delay(i);     
    myservoC.write(map(pos,1,sudu,A3-15,zzhou)); //让小臂(C) 从指定位置旋转到90度     使机械臂回到初始位置    
    delay(i);                        
  }  
 for(pos = 0; pos <=sudu; pos += 1) 
  {    
    myservoA.write(map(pos,1,sudu,A1,xzhou));// 让大臂（A）从指定位置旋转到109度 
    delay(i);                      
  }    

     
}





void loop() 
{ 
myservoA.write(xzhou);
  myservoB.write(yzhou);
    myservoC.write(zzhou);
  while (Serial.available() > 0)  
    {
        mycommand += char(Serial.read());
        delay(2);
    }
    if (mycommand.length() > 0)   
    
    {     
      
       commaPosition = mycommand.indexOf(',');
  int k=mycommand.indexOf("CJ(");
  int j=mycommand.lastIndexOf(")");
    String qpx = mycommand.substring(k+3,commaPosition);
     String qpy=mycommand.substring(commaPosition+1,j); 
     //分解成qpx,qpy   
   
  




//转化成取子电机角度

 char shujuqz[11];

    strcpy(shujuqz, qh[qzi]);
qzi=qzi+1;
if ( qzi>4)
{
  qzi=0;
  }


String d1qz = "";
    
    for(i=0;i<11;i++){
        d1qz += shujuqz[i];
    }





           commaPosition31qz = d1qz.indexOf(",");
           commaPosition41qz = d1qz.lastIndexOf(",");


 //    String tx=d1.substring(k31,j31-3); 
     String qx=d1qz.substring(0,commaPosition31qz);
    String qy = d1qz.substring( commaPosition31qz+1,commaPosition41qz); 
    String qz=d1qz.substring(commaPosition41qz+1,d1qz.length()); 
    

//取子坐标转化成电机角度完成    
     
     
     

//转化成投子电机角度
char shuju1[11];
    strcpy(shuju1, zb[qpx.toInt()][qpy.toInt()]);



String d1 = "";
    
    for(i=0;i<11;i++){
        d1 += shuju1[i];
    }





           commaPosition31 = d1.indexOf(",");
           commaPosition41 = d1.lastIndexOf(",");


 //    String tx=d1.substring(k31,j31-3); 
     String tx=d1.substring(0,commaPosition31);
    String ty = d1.substring( commaPosition31+1,commaPosition41); 
    String tz=d1.substring(commaPosition41+1,d1.length()); 
    

//投子坐标转化成电机角度完成         

      

     
 

     
     
     
//调用取子动作函数

runq(qx.toInt(),qy.toInt(),qz.toInt()); 

//调用放子动作函数
runt(tx.toInt(),ty.toInt(),tz.toInt()); 


       delay(0);

            mycommand="";

    }
            mycommand="";

}  

