#include "FIFO.h"
FIFO myBuffer;

char test[64] = "Hello, World!!!\nHello, World!!!\nHello, World!!!\nHello, World!!!";

void setup() {
    Serial.begin(9600);

  //Wait until the serial port has opened
  while (!Serial) delay(1);

  //Wait a little bit to make sure we don't get any garbage on the serial monitor
  delay(100);
}
void loop() {
  //Push the test array above onto the FIFO buffer
  for (byte i = 0; i < sizeof(test); i++) {
    Serial.write(test[i]);
    myBuffer.push(test[i]);
    delay(100);
  }

  Serial.write("\n\n");

  //Pop items off the FIFO buffer until it's empty
  while (myBuffer.size() > 0) {
    Serial.write((char)myBuffer.pop());
    delay(100);
  }

  while (1);
}
