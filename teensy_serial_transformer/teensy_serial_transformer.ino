#include "logs.h"
#include "automatic_transformer.h"
#include "universal_transformer.h"
#include "webserver.h"

#include <SPI.h>
#include <SD.h>
#include <string>
//#include <NativeEthernet.h>
#include <QNEthernet.h>
using namespace qindesign::network;



constexpr uint32_t kDHCPTimeout = 15000;  // 15 seconds
constexpr uint16_t kServerPort = 80;

// The IP address info will be dependent on your local network
// First 3 numbers must match router, last must be unique
// 
IPAddress ip{192, 168, 0, 107};   // Unique IP
IPAddress sn{255,255,255,0};  // Subnet Mask
IPAddress gw{10,0,0,1};       // Default Gateway
// Initialize the Ethernet server library with the IP address and port
// to use.  (port 80 is default for HTTP):
EthernetServer server(kServerPort);
EthernetServer* server_ptr = &server;


void setup() {

  Serial2.begin(9600);
  // Show whether a cable is plugged in or not.
  Ethernet.onLinkState([](bool state) {
    Serial2.printf("[Ethernet] Link %s\n", state ? "ON" : "OFF");
  });

  // Unlike the Arduino API (which you can still use), Ethernet uses
  // the Teensy's internal MAC address by default
  uint8_t mac[6];
  Ethernet.macAddress(mac);
  Serial2.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial2.println("Starting Ethernet with DHCP...");
  if (!Ethernet.begin()) {
    Serial2.println("Failed to start Ethernet");
    return;
  }
  if (!Ethernet.waitForLocalIP(kDHCPTimeout)) {
    Serial2.println("Failed to get IP address from DHCP");
  } else {
    IPAddress ip = Ethernet.localIP();
    Serial2.printf("    Local IP    = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.subnetMask();
    Serial2.printf("    Subnet mask = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.gatewayIP();
    Serial2.printf("    Gateway     = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.dnsServerIP();
    Serial2.printf("    DNS         = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);

    // Start the server
    Serial2.printf("Listening for clients on port %u...\n", kServerPort);
    server.begin();
  }
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial2.println("SD initialization failed!");
    while (1);
  }


  Serial.begin(9600);
  Serial3.begin(9600);

} 

// Example settings that could be toggled
bool translationEnabled = true;
bool loggingEnabled = true;
// Example of logs
String logs = "Log entry 1\nLog entry 2\nLog entry 3\n";
// Example of accumulators
int accumulator = 0;
// Global variables for RA and DEC
float ra = 0.0;  // Right Ascension
float dec = 0.0;  // Declination

SerialLogger logger;
SerialLogger* logger_ptr = &logger;
AutomaticSerialTransformer ast(1.0, 1.0, logger_ptr);
AutomaticSerialTransformer* ast_ptr = &ast;
TransformWebServer transform_webserver(server_ptr, ast_ptr, logger_ptr);

void loop() {
  // listen for incoming clients
  transform_webserver.handleWebServer();
  ast.readSerial();

}
// Function to extract query parameter from request string
String getQueryParam(String request, String param) {
  int paramIndex = request.indexOf(param + "=");
  if (paramIndex == -1) return "";
  paramIndex += param.length() + 1;
  int paramEnd = request.indexOf("&", paramIndex);
  if (paramEnd == -1) paramEnd = request.indexOf(" ", paramIndex);
  return request.substring(paramIndex, paramEnd);
}
// void setup_logging() {
//  throw "Not implemented";
// }

// void writeLog(std::string message) {
//   throw "Not implemented";
// }

// void readLog() {
//   throw "Not implemented";
// }