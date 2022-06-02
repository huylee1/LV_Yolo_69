#include <Thread.h>
#include <StaticThreadController.h>
#include <cppQueue.h>

#define valPin0 8
#define valPin2 9
#define sensor 10
#define sensor0 6
#define sensor2 7
#define MAX_LENGTH 50

int current_state = 1;
int previous_state = 1;
int state_sensor0 = 1;
int pre_sensor0 = 1;
int state_sensor2 = 1;
int pre_sensor2 = 1;

unsigned long interval_1=1000; // tgian phun 

unsigned long prevMillis0_ON=0; // millis() returns an unsigned long.
unsigned long prevMillis0_OFF=0;

unsigned long prevMillis2_ON=0;
unsigned long prevMillis2_OFF=0;

bool check0 = false;
bool check2 = false;

cppQueue  q(sizeof(char), MAX_LENGTH, FIFO, true);

//goi ham cho Thread

void sensor_func(){
  current_state = digitalRead(sensor);
  if (current_state != previous_state) {
    if (current_state == HIGH)
      Serial.println("falling");
    if (current_state == LOW)
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
    q.peekIdx(&label,0);                //class (0: bad, 1:good1, 2:good2)
    state_sensor0 = digitalRead(sensor0);
//    if ((state_sensor0 != pre_sensor0) && (state_sensor0 == LOW)){
      if (state_sensor0 != pre_sensor0){
        if(state_sensor0 == LOW){
      

      if (label == '0') {
        digitalWrite(valPin0, HIGH); 
        Serial.println("valve 0 ON");
        check0 = true;
        prevMillis0_ON = millis();
        q.pop(&label);
      }
      else if (label == '1'){
        q.pop(&label);
        Serial.println("good1");
      }
    }
      }
    pre_sensor0 = state_sensor0;
    if (check0)
      if ((unsigned long)(millis() - prevMillis0_ON) >= interval_1){
        digitalWrite(valPin0, LOW); 
        Serial.println("valve 0 OFF");
        check0 = false;
        prevMillis0_ON = millis();
      }
    q.peekIdx(&label,0);
    state_sensor2 = digitalRead(sensor2);
//    if ((state_sensor2 != pre_sensor2) && (state_sensor2 == LOW)){
      if (state_sensor2 != pre_sensor2){
        if(state_sensor2 == LOW){
     
      if (label == '2') {
        digitalWrite(valPin2, HIGH); 
        Serial.println("valve 2 ON");
        check2 = true;
        prevMillis2_ON = millis();
        q.pop(&label);
      }
      else if (label == '1'){
        q.pop(&label);
        Serial.println("good1");
      }
    }
      }
    
    pre_sensor2 = state_sensor2;
    if (check2)
      if ((unsigned long)(millis() - prevMillis2_ON) >= interval_1){
        digitalWrite(valPin2, LOW); 
        Serial.println("valve 2 OFF");
        check2 = false;
        prevMillis2_ON = millis();
      }
  }
  if (check0)
    if ((unsigned long)(millis() - prevMillis0_ON) >= interval_1){
      digitalWrite(valPin0, LOW); 
      Serial.println("valve 0 OFF");
      check0 = false;
      prevMillis0_ON = millis();
    }
  if (check2)
    if ((unsigned long)(millis() - prevMillis2_ON) >= interval_1){
      digitalWrite(valPin2, LOW); 
      Serial.println("valve 2 OFF");
      check2 = false;
      prevMillis2_ON = millis();
    }
}

Thread read_sensor = Thread(sensor_func);
Thread rec_signal = Thread(signal_func);
StaticThreadController<2> controll (&read_sensor, &rec_signal);

void setup(){
  pinMode(sensor, INPUT_PULLUP);
  pinMode(sensor0, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  pinMode(valPin0, OUTPUT);
  pinMode(valPin2, OUTPUT);
  
  Serial.begin(9600);  
  digitalWrite(valPin0, LOW);
  digitalWrite(valPin2, LOW);
  q.flush();
  
//  read_sensor.onRun(sensor_func);
//  read_sensor.setInterval(150);
//  rec_signal.setInterval(400);
}


void loop(){ 
  controll.run();
}
    
