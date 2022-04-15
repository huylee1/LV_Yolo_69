int x;
void setup() {
 Serial.begin(9600);
 Serial.setTimeout(1); //thời gian tối đa chờ cho serial data (milis)
}
void loop() {
 while (!Serial.available());
 x = Serial.readString().toInt(); //đọc dữ liệu từ máy tính
 Serial.print(x + 1);
}
