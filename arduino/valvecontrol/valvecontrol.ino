#define valPin0 8
#define valPin2 9
#define sensor 10
#include <Thread.h>
#include <StaticThreadController.h>
#include <cppQueue.h>

#define  MAX_LENGTH    50
int current_state = 0;
int previous_state = 1;


bool check1 = false;
bool check2 = false;
unsigned long interval_1=1000; // the time we need to wait
unsigned long interval_2=2000;
unsigned long interval_4=4000;
unsigned long prevMillis_ON=0; // millis() returns an unsigned long.
unsigned long prevMillis_OFF=0;

cppQueue  q(sizeof(char), MAX_LENGTH, FIFO, true);

//goi ham cho Thread

void sensor_func(){
    current_state = digitalRead(sensor);
  if(current_state!=previous_state)
  { if(current_state == HIGH)
      Serial.println("falling");
    if(current_state == LOW)
      Serial.println("rising");
  }
      previous_state = current_state;
}


void signal_func(){
  unsigned long currentMillis = millis(); 
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
    char label;
    q.peekIdx(&label,0);
    
    if (label == '0') { // neu toi la "bad"                           //class (0: bad, 1:good1, 2:good2)
      if ((unsigned long)(currentMillis - prevMillis_ON) >= interval_2) {
        prevMillis_ON = millis();
        if (check1 == true){
          q.pop(&label);
          digitalWrite(valPin0, HIGH); // valve0 ON after 2s
          Serial.println("valve 0 ON");
          check2 = true;
          prevMillis_OFF = millis();
          check1 = false;
        }
        check1 = true;
      }
    }
    else {
      q.pop(&label);
      Serial.println("good1");
    }
      if ((check2) && ((unsigned long)(currentMillis - prevMillis_OFF) >= interval_1)) { 
        digitalWrite(valPin0, LOW); // valve0 OFF after 1s
        prevMillis_OFF = millis();
        Serial.println("valve 0 OFF");
        check2 = false;
      }


    ///////////////////////////////////////
//    if (label == '0') { // neu toi la "bad"                           //class (0: bad, 1:good1, 2:good2)
//      q.pop(&label);
//      if ((unsigned long)(currentMillis - prevMillis_ON) >= interval_2) {
//        digitalWrite(valPin0, HIGH); // valve0 ON after 2s
//        prevMillis_ON = millis();
//        Serial.println("valve 0 ON");
//      }
//      if ((unsigned long)(currentMillis - prevMillis_OFF) >= interval_1) { 
//        digitalWrite(valPin0, LOW); // valve0 OFF after 1s
//        prevMillis_OFF = millis();
//        Serial.println("valve 0 OFF");
//      }
//    }
   //////////////////////////////////////////// 
   
//    else if (label == '2') { // neu toi la "good2"
//      q.pop(&label);
//      digitalWrite(valPin2, LOW); // valve2 OFF for 4s
//      delay(4000);
//      digitalWrite(valPin2, HIGH); // valve2 ON for 1s
//      delay(1000);
//      Serial.write("valve2 da thoi");
//    } 

    else {
      q.pop(&label);
      Serial.write("good1");
    }
  }
}

Thread read_sensor = Thread();
Thread rec_signal = Thread(signal_func);
StaticThreadController<2> controll (&read_sensor, &rec_signal);

void setup(){
  pinMode(sensor, INPUT);
  pinMode(valPin0, OUTPUT);
  pinMode(valPin0, OUTPUT);
//  Serial.begin(115200);  
  Serial.begin(9600);  

  read_sensor.onRun(sensor_func);
  read_sensor.setInterval(150);

//  rec_signal.setInterval(400);
}


void loop(){ 
  controll.run();
}
    
