//#include <NativeEthernet.h>
#include <Arduino.h>
#include <QNEthernet.h>
#include <SPI.h>
#include <SD.h>
using namespace qindesign::network;
// HTML page as a string

//qn::EthernetClient client;
class TransformWebServer {
public:
  EthernetServer* server;
  AutomaticSerialTransformer* ast;
  SerialLogger* logger;
  bool translationEnabled = true;
  bool loggingEnabled = true;
  bool selectedMode = true;

  int accumulator = 0;


  float ra = 0.0;   // Right Ascension
  float dec = 0.0;  // Declination
  TransformWebServer(EthernetServer* ptr_server, AutomaticSerialTransformer* ptr_ast, SerialLogger* ptr_logger) { 
    server = ptr_server;
    ast = ptr_ast;
    logger = ptr_logger;
    logger->log("Web server initialized.");
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


  void handleWebServer() {
    EthernetClient client = server->available();
    if (client) {
      String request = "";
      Serial.println("new client");
      // an http request ends with a blank line
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          //Serial.println(c);
          request += c;
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n') {
            // Check for specific requests
            // Handle the new camera_api endpoint
            if (request.indexOf("GET /camera_api") != -1) {
              String password = getQueryParam(request, "passwd");         // zmenit podla requestu
              String pixelCount = getQueryParam(request, "accel_const");  // zmenit podla requestu
              if (password == "12345") {            // zadat spravne heslo
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><body>");
                client.println("<h1>Request successful.</h1>");
                client.println("<p>Value: " + pixelCount + "</p>");
                client.println("<button onclick=\"window.history.back();\">Back</button>");
                client.println("</body></html>");

                int pixCount = pixelCount.toInt();
                Serial.print("Received Pixel Count: ");
                Serial.println(pixCount);
                ast->addError(pixCount);
              } else {
                client.println("HTTP/1.1 403 Forbidden");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><body>");
                client.println("<h1>Unauthorized</h1>");
                client.println("<p>Incorrect password.</p>");
                client.println("<button onclick=\"window.history.back();\">Back</button>");
                client.println("</body></html>");
              }
            } else if (request.indexOf("GET /set_mode") != -1) {
              String mode = getQueryParam(request, "mode");
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html><body>");
              client.println("<h1>Mode Set: " + mode + "</h1>");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
              // Update the mode in AutomaticSerialTransformer
              if (mode == "auto") {
                  selectedMode = true;
                  ast->setMode(true);
              } else if (mode == "manual") {
                  selectedMode = false;
                  ast->setMode(false);
              }
            } else if (request.indexOf("GET /log_action?command=show") != -1) {
              // Show logs action
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html><head><title>Logs</title></head><body>");
              client.println("<h2>Logs</h2><pre>");
              String logs = logger->read_logs(0);
              //go thorgh every char in logs and send it to client
              for (u_int i = 0; i < logs.length(); i++) {
                client.print(logs[i]);
              }
              client.println("</pre>");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
            } else if (request.indexOf("GET /log_action?command=clear") != -1) {
              // Clear logs action
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              logger->delete_all_logs();
              client.println("<html><body>Logs cleared.");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
            } else if (request.indexOf("GET /download_logs") != -1) {
              // Download all logs action
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");                            // This makes the browser treat it as a text file
              client.println("Content-Disposition: attachment; filename=logs.txt");  // Prompt file download
              client.println("Connection: close");
              client.println();
              String logs = logger->read_logs(0);
              //go thorgh every char in logs and send it to client
              for (u_int i = 0; i < logs.length(); i++) {
                client.print(logs[i]);
              }
            } else if (request.indexOf("GET /reset_accumulators") != -1) {
              // Reset accumulators action
              accumulator = 0;  // Reset the accumulator
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html><body>Accumulators reset.");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
            }
            // Toggle translationEnabled
            else if (request.indexOf("GET /toggle_translation") != -1) {
                translationEnabled = !translationEnabled;  // Toggle the value
                ast->setTranslationEnabled(translationEnabled);   // Update the value in AutomaticSerialTransformer
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><body>");
                client.println("<h1>Translation " + String(translationEnabled ? "Enabled" : "Disabled") + "</h1>");
                client.println("<button onclick=\"window.history.back();\">Back</button>");
                client.println("</body></html>");
            }
            // Toggle loggingEnabled
            else if (request.indexOf("GET /toggle_logging") != -1) {
                loggingEnabled = !loggingEnabled;  // Toggle the value
                ast->setLoggingEnabled(loggingEnabled);   // Update the value in AutomaticSerialTransformer
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("Connection: close");
                client.println();
                client.println("<html><body>");
                client.println("<h1>Logging " + String(loggingEnabled ? "Enabled" : "Disabled") + "</h1>");
                client.println("<button onclick=\"window.history.back();\">Back</button>");
                client.println("</body></html>");
            }
            // Returns JSON for sliders on webpage
            else if (request.indexOf("GET /get_current_settings") != -1) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: application/json");
                client.println("Connection: close");
                client.println();
                client.print("{ \"prekladanie\": \"");
                client.print(translationEnabled ? "enabled" : "disabled");
                client.print("\", \"logovanie\": \"");
                client.print(loggingEnabled ? "enabled" : "disabled");
                client.print("\", \"mode\": \"");
                client.print(selectedMode ? "auto" : "manual");
                client.println("\" }");
            }
            // Handle form submissions for RA and DEC
            else if (request.indexOf("GET /?ra=") != -1) {
              ra = getQueryParam(request, "ra").toFloat();  // Convert RA to double
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html><body>RA Submitted: " + String(ra) + "");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
              ast->setRaRatio(ra);
            } else if (request.indexOf("GET /?dec=") != -1) {
              dec = getQueryParam(request, "dec").toFloat();  // Convert DEC to double
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              client.println("<html><body>DEC Submitted: " + String(dec) + "");
              client.println("<button onclick=\"window.history.back();\">Back</button>");
              client.println("</body></html>");
              ast->setDecRatio(dec);
            }
            // Send the HTML page content for root request
            else if (request.indexOf("GET /") != -1) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.println();
              //open file web.html from sd card and send it to client
              File myfile = SD.open("web.html");
              if (myfile) {
                String web_line = "";
                while (myfile.available()) {
                  char charakter = myfile.read();
                  client.print(charakter);
                  
                }
                myfile.close();
              } else {
                client.println("<html><body><h1>File not found!</h1></body></html>");                
              }
            }
            // End the response
            break;
          }
        }
      }
      client.stop();  // Stopping because we specified "Connection: close"
      Serial.println("client disconnected");
    }
  }
}
;