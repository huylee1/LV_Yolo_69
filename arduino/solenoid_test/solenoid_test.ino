#define mosfetPin 8
void setup()  {
  pinMode(mosfetPin, OUTPUT);
 
}
void loop() {
  digitalWrite(mosfetPin, LOW);
  delay(1000);
  digitalWrite(mosfetPin, HIGH);
  delay(1000);
}
