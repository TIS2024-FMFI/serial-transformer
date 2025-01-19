//#include <NativeEthernet.h>
#include <Arduino.h>
#include <QNEthernet.h>
using namespace qindesign::network;
#include <SPI.h>
// HTML page as a string

//qn::EthernetClient client;
class TransformWebServer{
public:
  EthernetServer* server;
  bool translationEnabled = true;
  bool loggingEnabled = true;

  String logs = "Log entry 1\nLog entry 2\nLog entry 3\n";

  int accumulator = 0;


  float ra = 0.0;  // Right Ascension
  float dec = 0.0;  // Declination


  String htmlPage = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Test Webpage</title>
  </head>
  <body>
      <h1>Testing</h1>

      <form id="form1" action="#" method="get">
          <label for="ra">RA:</label>
          <input type="number" id="ra" name="ra">
          <button type="submit">Submit RA</button>
      </form>

      <form id="form2" action="#" method="get">
          <label for="dec">DEC:</label>
          <input type="number" id="dec" name="dec">
          <button type="submit">Submit DEC</button>
      </form>

      <button id="buttonUkazatLogy">Ukazat logy</button>
      <button id="buttonZmazatLogy">Zmazat vsetky logy</button>
      <button id="buttonStiahnutLogy">Stiahnut vsetky logy</button>
      <button id="buttonResetAccumators">Reset accumators</button>

      <div>
          <label for="togglePrekladanie">Deaktivacia prekladania</label>
          <label class="switch">
              <input type="checkbox" id="togglePrekladanie">
              <span class="slider"></span>
          </label>
      </div>

      <div>
          <label for="toggleLogovanie">Deaktivacia logovania</label>
          <label class="switch">
              <input type="checkbox" id="toggleLogovanie">
              <span class="slider"></span>
          </label>
      </div>
      
      <script>
          // Button Actions for GET requests
          document.getElementById("buttonUkazatLogy").addEventListener("click", function() {
              fetch("/log_action?command=show")
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);
                  });
          });

          document.getElementById("buttonZmazatLogy").addEventListener("click", function() {
              fetch("/log_action?command=clear")
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);
                  });
          });

          // Button Actions for Downloading Logs
          document.getElementById("buttonStiahnutLogy").addEventListener("click", function() {
              fetch("/download_logs")
                  .then(response => response.blob())
                  .then(data => {
                      const url = window.URL.createObjectURL(data);
                      const a = document.createElement('a');
                      a.href = url;
                      a.download = 'logs.txt';  // Specify the filename for download
                      a.click();
                      window.URL.revokeObjectURL(url);
                  });
          });

          // Button Actions for Resetting Accumulators
          document.getElementById("buttonResetAccumators").addEventListener("click", function() {
              fetch("/reset_accumulators")
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);  // Handle the reset confirmation response
                  });
          });

          // Toggle Actions for GET requests
          document.getElementById("togglePrekladanie").addEventListener("change", function() {
              const status = this.checked ? "enabled" : "disabled";
              fetch(`/toggle_translation?status=${status}`)
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);
                  });
          });

          document.getElementById("toggleLogovanie").addEventListener("change", function() {
              const status = this.checked ? "enabled" : "disabled";
              fetch(`/toggle_logging?status=${status}`)
                  .then(response => response.text())
                  .then(data => {
                      console.log(data);
                  });
          });
      </script>
  </body>
  </html>
  )rawliteral";
  TransformWebServer(EthernetServer* ptr_server) {
    server = ptr_server;
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
    boolean currentLineIsBlank = true;
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
          if (request.indexOf("GET /log_action?command=show") != -1) {
            // Show logs action
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<html><body>Logs shown here.</body></html>");
          } 
          else if (request.indexOf("GET /log_action?command=clear") != -1) {
            // Clear logs action
            logs = "";  // Reset logs
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<html><body>Logs cleared.</body></html>");
          } 
          else if (request.indexOf("GET /download_logs") != -1) {
            // Download all logs action
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");  // This makes the browser treat it as a text file
            client.println("Content-Disposition: attachment; filename=logs.txt");  // Prompt file download
            client.println("Connection: close");
            client.println();
            client.println(logs);  // Send the logs as the file content
          } 
          else if (request.indexOf("GET /reset_accumulators") != -1) {
            // Reset accumulators action
            accumulator = 0;  // Reset the accumulator
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<html><body>Accumulators reset.</body></html>");
          }
          // Handle form submissions for RA and DEC
          else if (request.indexOf("GET /?ra=") != -1) {
            ra = getQueryParam(request, "ra").toFloat();  // Convert RA to float
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<html><body>RA Submitted: " + String(ra) + "</body></html>");
          } 
          else if (request.indexOf("GET /?dec=") != -1) {
            dec = getQueryParam(request, "dec").toFloat();  // Convert DEC to float
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<html><body>DEC Submitted: " + String(dec) + "</body></html>");
          }
          // Send the HTML page content for root request
          else if (request.indexOf("GET /") != -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.print(htmlPage);
          }
          // End the response
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    client.stop();  // Stopping because we specified "Connection: close"
    Serial.println("client disconnected");
  }
  }

};

















