#include <TimerOne.h>
////////////////////////////////// Nguyen van Hau 10/06/2021//////////////////////////////////////
//#define ledPin 13
//#define ledPin1 6
//#define ledPin2 8
//#define ledPin3 9
#define valPin1 7
#define valPin2 8
#define valPin3 9
const int phase_a = 2;
const int phase_b = 3;
#include <ArduinoJson.h>
#include <arduino.h>
#include <HardwareSerial.h>
int pulse = 0;
int pulsecheck1=0;
int pulsecheck2=0;
int pulsecheck3=0;
byte khunghinh[16]={0};
byte index_khung=0;
byte index_khung_do=0;
String data="";
String bff="";
String point="";
char check ="";
long sumofPOINT =0;
unsigned int oldpulse1=0;
unsigned int oldpulse2=0;
int bfrv_lo[21]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bfrv_sta[21]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long myTime;
bool val_on1 =false;
bool val_on2 =false;
bool val_on3 =false;
bool checkon=false;
bool dontupdate=false;
int ON=-1;
byte time_cnt=0;
// index of buffer
unsigned int bfdo_index=0;
unsigned int bfrv_index=0;
int store;
unsigned long time1;
void setup() {
  Serial.begin(115200);
  store =false;
//  pinMode(ledPin, OUTPUT);
  pinMode(valPin1, OUTPUT);
  pinMode(valPin2, OUTPUT);
  pinMode(valPin3, OUTPUT);
  attachInterrupt(0, encoder,FALLING);  
  pinMode (phase_a, INPUT_PULLUP);
  pinMode (phase_b, INPUT_PULLUP);
//  while(!Serial) {
//  }
Timer1.initialize(5000); // khởi động ngắt, thời gian đặt là 5000us = 0.005s
Timer1.attachInterrupt(Blink); //khi xay ra ngắt thì gọi hàm Blink
//      
}
void sort(int valve){
    if (valve==0){
    digitalWrite(valPin1, HIGH);
    val_on1 =true;
    pulsecheck1 = time_cnt;
    }
    
    if (valve==1){
      digitalWrite(valPin2, HIGH);
    val_on2 =true;
    pulsecheck2=time_cnt;
    }
    if (valve==2){
      digitalWrite(valPin3, HIGH);
     val_on3 =true;
     pulsecheck3=time_cnt;
    }
    time_cnt++;
    }
void encoder(){
  if ( digitalRead (phase_b)==LOW)
  {
    pulse--;
    
    } 
    else 
    {
     pulse++; 
      }
      if(store==true){
if(checkon==true){
  if (((pulse<=bfrv_lo[bfdo_index])&&(pulse>=bfrv_lo[bfdo_index]-10))||(bfrv_lo[bfdo_index]==-1)){ // tuy chinh he số để tăng khoảng cách bắn
    if(!dontupdate){
    ON=khunghinh[index_khung_do]-1;}
    if(ON>=0){
      ON--;
      dontupdate=true;
      sort(bfrv_sta[bfdo_index]);
      bfrv_sta[bfdo_index]=0;
      bfrv_lo[bfdo_index]=0;
      bfdo_index++;
        if(bfdo_index>19){
    bfdo_index =0;
  }
    }
    if(ON<0){checkon=false;
    dontupdate=false;
    index_khung_do++;
//    bfdo_index++;
    if(index_khung_do>15){index_khung_do=0;}}
  }
//  if(abs(bfrv_index-bfdo_index)<1){
//    bfdo_index=bfdo_index-2;
//}
}
 
}
//if(abs(bfrv_index-bfdo_index)<(15*khunghinh[index_khung_do])&&abs(bfrv_index-bfdo_index)>(4*khunghinh[index_khung_do])){
//  bfdo_index=bfdo_index+2;
//     if(bfdo_index>19){
//      bfdo_index=0;
//     }
//}
//Serial.println("thoigian:");
// Serial.println(millis()-time1);

}

void serialEvent(){
  if(Serial.available()>0){
    bff = Serial.readStringUntil('*');
    bff.trim();
    data += bff;
    bff="";
       if(data.indexOf('!')>=0){
    pulse=0;
    checkon=true;
    data.remove(data.indexOf('!'),1); 
  }
  }
}
void loop() { 
  if(data.indexOf('c')>=0){
//  Serial.print("abc: ");
//  Serial.println(data);
    sumofPOINT= (data.substring(data.indexOf('c')+1,data.length())).toInt();
    data.remove(data.indexOf('c'),1);
    if(sumofPOINT==0){khunghinh[index_khung]=1;
    }
    else{
    khunghinh[index_khung]=sumofPOINT;}
    index_khung++;
    if(index_khung>15){index_khung =0;}
    if(store==false){
    if (index_khung>3){store = true;}}
//  Serial.print("abc: ");
//  Serial.println(data);
  }
  if (data.indexOf('l')>=0){
//      Serial.print("abc: ");
//  Serial.println(sumofPOINT);
    if(sumofPOINT==0){
      bfrv_lo[bfrv_index]=-1;
     bfrv_sta[bfrv_index]=-1;
       bfrv_index++;
       if(bfrv_index>19){
      bfrv_index=0;
     }
    }
    else{
    for(int i =0;i<sumofPOINT; i++){
 
     
     int a= data.indexOf('[');
     int b= data.indexOf(']');
     point = data.substring(a+1,b);
     int c= point.indexOf(',');
     int get_sta = (point.substring(0,c)).toInt();
     int get_lo =(point.substring(c+1,point.length())).toInt();
//      Serial.print("check location:");
//     Serial.println(get_lo);
     bfrv_lo[bfrv_index]=get_lo;
     bfrv_sta[bfrv_index]=get_sta;
//    Serial.println("location: ");
//     Serial.println(bfrv_lo[bfrv_index]);
      bfrv_index++;
         if(bfrv_index>19){
      bfrv_index=0;
     }
     data.remove(a,b-a+1); 
//      Serial.print("sta: ");
//     Serial.println(get_sta);
//     Serial.print("lo: ");
//     Serial.println(get_lo);
    }}
    data="";
  }

//Serial.print("array : ");
//  Serial.print(bfrv_lo[bfdo_index]);
//  Serial.println("array");
//Serial.print("check pulse");
//Serial.println(pulse);
//Serial.print("check index");
//Serial.println(bfrv_lo[bfdo_index]);
if(val_on1){
  if(pulsecheck1==time_cnt){
     digitalWrite(valPin1, LOW);
    val_on1 =false;
  }
}
if(val_on2){
  if(pulsecheck2==time_cnt){
     digitalWrite(valPin2, LOW);
    val_on2 =false;
  }
}
if(val_on3){
  if(pulsecheck3==time_cnt){
     digitalWrite(valPin3, LOW);
    val_on3 =false;
  }
}

  //  data="!*c1 *l(([   1, 60],)*";
  //  data="!*c0 *l[]";
  //"!*c2 *l(([   1, 86]), ([   1, 96]),*"
   //"!*c2 *l(([   1, 50]), ([   1, 300]),*"
//"!*c4 *l(([   1, 86]), ([   1, 96]), ([   1, 186]), ([   1, 286]))*"
//"!*c7*l(([   0, 300]), ([   0, 350]), ([   0, 350]), ([   1, 350]), ([   0, 360]), ([   0, 365]), ([   0, 366]))*"
  ///////////////////////////////////////
  //// process string with arduino///////
  ///////////////////////////////////////
//   myTime = millis();
//////
//Serial.print("location now : ");
//  Serial.println(bfrv_lo[bfdo_index]-20);
//  Serial.println("array");
////  Serial.print(" pulse: ");
////  Serial.println(pulse);
//Serial.print(" index of lo: ");
//  Serial.println(bfrv_index);
//Serial.print(" index of do: ");
//  Serial.println(bfdo_index);
//    Serial.print("V = ");
//  Serial.print((oldpulse1-oldpuls e2)/(0.202101515));
//  Serial.println("mm/s");
//  Serial.print(" array in location: ");
//  Serial.println(bfrv_lo[i]);
//  }
//  Serial.print("check khung hinh:");
//  Serial.println(index_khung);
//   Serial.print("check khung hinh do:");
//  Serial.println(index_khung_do);
//Serial.println(ON);
Serial.println(pulse);
//Serial.println(store);
//Serial.print("print:");
//Serial.println(ON);
//Serial.print("dont update:");
//Serial.println(dontupdate);
//Serial.println(checkon);

}
  void Blink()
{ 
  time_cnt++;
  if(time_cnt>=50){
    time_cnt=0;
  }
}
  
  
