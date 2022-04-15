#define sensor 8
byte pre = 1;
String chuoi;
void setup()
{
  pinMode(sensor,INPUT);
  Serial.begin(9600);
}
void loop()
{
  Serial.print("Gia tri cam bien:");
  chuoi = String(digitalRead(sensor)) + String(pre);
  Serial.println(chuoi);
//  Serial.println(digitalRead(sensor)); //Nếu sensor = 1 thì không phát hiện vật cản, nếu sensor = 0 thì phát hiện vật cản.
  pre = digitalRead(sensor);
  delay(150);
  //Serial.println(millis());

//nhan du lieu tu may tinh
//if (Serial.available()>0) {//khi nhan du lieu tu may tinh
//  String str = Serial.readString();
//  Serial.println(str);}
}
//  if (str == String("good2") {//neu la good2
    
//  }
  
//}
