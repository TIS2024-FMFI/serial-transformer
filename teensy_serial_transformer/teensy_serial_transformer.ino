#include "automatic_serial_transformer.h"

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

}
AutomaticSerialTransformer ast(1.0, 1.0);
void loop() {
  ast.readSerial();

}
