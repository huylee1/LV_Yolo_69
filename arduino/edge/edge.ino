int current_state = 0;
int previous_state = 1;
#define sensor 10
void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
//  Serial.println("xung la: ");
  current_state = digitalRead(sensor);
  if(current_state!=previous_state)
  { if(current_state == HIGH)
      Serial.println("falling");
    if(current_state == LOW)
      Serial.println("rising");
  }
      previous_state = current_state;
}
