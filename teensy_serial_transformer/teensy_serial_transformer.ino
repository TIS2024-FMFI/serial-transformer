#include "automatic_transformer.h"
#include "universal_transformer.h"
#include "webserver.h"
#include <SPI.h>
#include <NativeEthernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 126 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

void setup() {
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

}
AutomaticSerialTransformer ast(1.0, 1.0);

void loop() {
  handleWebServer(server)
  ast.readSerial();

}

void setup_logging() {
 throw "Not implemented";
}

void writeLog(string message) {
  throw "Not implemented";
}

void readLog() {
  throw "Not implemented";
}