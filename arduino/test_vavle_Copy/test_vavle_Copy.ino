//#define mosfetPin 9
//void setup()  {
//  pinMode(mosfetPin, OUTPUT);
// 
//}
//void loop() {
//  digitalWrite(mosfetPin, LOW);
//  delay(1000);
//  digitalWrite(mosfetPin, HIGH);
//  delay(1000);
//}

///////////////////////
#define valPin0 8
#define valPin2 9
#define sensor 10
byte pre = 1;
String chuoi;
#include <HardwareSerial.h>
//#include <ArduinoJson.h>
//#include <arduino.h>
//
//bool val_on0 =false;
//bool val_on2 =false;

#include <cppQueue.h>

#define  MAX_LENGTH    50
#define EOT_CHAR    ';'

cppQueue  q(sizeof(char), MAX_LENGTH, FIFO, true);

void setup(){
  pinMode(sensor, INPUT);
  pinMode(valPin0, OUTPUT);
  pinMode(valPin0, OUTPUT);
  Serial.begin(115200);  
//  Serial.flush();
}

void loop() //doc du lieu tu sensor
{

//  Serial.print("Gia tri cam bien:");
//  Serial.println(digitalRead(sensor)); //Nếu sensor = 1 thì không phát hiện vật cản, nếu sensor = 0 thì phát hiện vật cản.
//  delay(50);

//    Serial.print("Gia tri cam bien:");
//    chuoi = String(digitalRead(sensor)) + String(pre);
//    Serial.println(chuoi);
//    pre = digitalRead(sensor);
//    delay(150);

//  if (Serial.available()>0)  {
//    String label = Serial.readString();
//    // queue ngay day!!!
////    label = Serial.read();
  if (Serial.available())          // Ensure Serial is available
  {
    char rcv = (char) Serial.read();  // Read char from Serial

    if (rcv != -1)            // If no character received: -1 (do not push)
    {
      q.push(&rcv);         // Push char to cppQueue    [012012012] if .... -> lấy số 1 ra ->kích hoạt vòi 1
    }
  }
  if (!q.isEmpty())        // Only if q is not empty
  {
    char snd, label;

    q.peekIdx(&label,0);
//truyen label lien tuc thi lam sao ?? 
    if (label == '0') { // neu toi la "bad"                           //class (0: bad, 1:good1, 2:good2)
//      Serial.println("valve0 da thoi");
      q.pop(&label);
      delay(1000);
      digitalWrite(valPin0, HIGH); // valve0 OFF for 2s 
//      Serial.println("waiting");

      delay(2000);
      digitalWrite(valPin0, LOW); // valve0 ON for 1s
//      delay(1000);
      Serial.write("valve0 da thoi");
    }

    
    else if (label == '2') { // neu toi la "good2"
//      Serial.println("valve2 da thoi");
      q.pop(&label);
      digitalWrite(valPin2, LOW); // valve2 OFF for 4s
//      Serial.println("waiting");
      delay(4000);
      digitalWrite(valPin2, HIGH); // valve2 ON for 1s
      delay(1000);
      Serial.write("valve2 da thoi");
    } 

    
    else {
      q.pop(&label);
      Serial.write("ke no");
    }
  }
} //solenoid airtac 2/2
// rờ le + valve + arduino + nguồn 
