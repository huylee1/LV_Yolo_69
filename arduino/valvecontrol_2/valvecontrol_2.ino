#define valPin0 8
#define valPin2 9
#define sensor 10
#include <Thread.h>
#include <StaticThreadController.h>
#include <cppQueue.h>

#define  MAX_LENGTH    50

int current_state;
int previous_state = 1;

unsigned long interval_1=3000; // tgian phun 
unsigned long interval_2=5000; // sau 5s bat valve 0
unsigned long interval_4=4000; // on van 2

unsigned long prevMillis0_ON=0; // millis() returns an unsigned long.
unsigned long prevMillis0_OFF=0;

unsigned long prevMillis2_ON=0; // millis() returns an unsigned long.
unsigned long prevMillis2_OFF=0;


bool check0_1 = false;
bool check0_2 = false;
bool check2_1 = false;
bool check2_2 = false;

cppQueue  q(sizeof(char), MAX_LENGTH, FIFO, true);

//goi ham cho Thread

void sensor_func(){
  current_state = digitalRead(sensor);
  if (current_state != previous_state) {
    if (current_state == HIGH)
      Serial.println("Falling edge");
    if (current_state == LOW)
      Serial.println("Rising edge");
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
      if ((unsigned long)(currentMillis - prevMillis0_ON) >= interval_2) {
//        Serial.println(prevMillis_ON);
        prevMillis0_ON = millis();
//        Serial.println(prevMillis_ON);
        if (check0_1 == true){
          prevMillis0_ON = millis();
          q.pop(&label);
          digitalWrite(valPin0, HIGH); // valve0 ON after 2s
          Serial.println("valve 0 ON");
          check0_2  = true;
          prevMillis0_OFF = millis();
          check0_1 = false;
        }
        else check0_1 = true;
      }
    }
    else if (label == '2') {
    if ((unsigned long)(currentMillis - prevMillis2_ON) >= interval_4) {
        prevMillis2_ON = millis();
        if (check2_1 == true){
          prevMillis2_ON = millis();
          q.pop(&label);
          digitalWrite(valPin2, HIGH); 
          Serial.println("valve 2 ON");
          check2_2  = true;
          prevMillis2_OFF = millis();
          check2_1 = false;
        }
        else check2_1 = true;
      }
    }
    else {
      q.pop(&label);
      Serial.println("good1");
    }
  }
  if (check0_2 == true)
    if((unsigned long)(currentMillis - prevMillis0_OFF) >= interval_1) { 
        digitalWrite(valPin0, LOW); // valve0 OFF after 1s
        prevMillis0_OFF = millis();
        Serial.println("valve 0 OFF");
        check0_2 = false;
    }
    if (check2_2 == true)
    if((unsigned long)(currentMillis - prevMillis2_OFF) >= interval_1) { 
        digitalWrite(valPin2, LOW); // valve2 OFF after 1s
        prevMillis2_OFF = millis();
        Serial.println("valve 2 OFF");
        check2_2 = false;
    }
}

Thread read_sensor = Thread(sensor_func);
Thread rec_signal = Thread(signal_func);
StaticThreadController<2> controll (&read_sensor, &rec_signal);

void setup(){
  pinMode(sensor, INPUT);
  pinMode(valPin0, OUTPUT);
  pinMode(valPin2, OUTPUT);
  Serial.begin(9600);  
  digitalWrite(valPin0, LOW);
//  read_sensor.onRun(sensor_func);
//  read_sensor.setInterval(150);

//  rec_signal.setInterval(400);
}


void loop(){ 
  controll.run();
}
    
