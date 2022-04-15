str x;
void setup() {
 Serial.begin(9600);
 Serial.setTimeout(1); //thời gian tối đa chờ cho serial data (milis)
}
void loop() {
 while (!Serial.available());
 x = Serial.readString().toInt(); //đọc dữ liệu từ máy tính
 if x = 0 {
  //vòi 0 phun
  Serial.print("activate vòi 1")
 }
 if x = 2 {
  //vòi 2 phun
  Serial.print("activate vòi 2")
 }
 else {
  pass
  Serial.print("no activate")
 }
 
}
