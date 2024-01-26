// AI Description:
// The selected code is part of an Arduino sketch that uses the ESP8266WiFi library to create a web server that listens for incoming connections on port 80. The code defines several functions that are used to handle different requests to the server, including the root page, the form submission page, and a 404 page for requests that cannot be matched to a specific handler.
// The code starts by including several libraries, including ESP8266WiFi, WiFiClient, ESP8266WebServer, and ESP8266mDNS. These libraries are used to communicate with the Wi-Fi network and to create the web server.
// The code then defines several constants, including the Wi-Fi network credentials, the pin number for the on-board LED, and a constant for the HTML code for the form submission page.
// The setup function starts by setting the pin for the on-board LED to output and setting the LED to off. It then initializes the Serial port and attempts to connect to the Wi-Fi network defined by the SSID and password constants.
// The while loop continues to loop while the Wi-Fi connection is not established. The delay function is used to wait for a short period of time before trying to connect to the Wi-Fi network again.
// Once the Wi-Fi connection is established, the code checks if the mDNS responder is running. If it is not running, the code starts the mDNS responder with the name "esp8266".
// The code then defines several functions that are used to handle different requests to the web server. The handleRoot function sends the HTML for the form submission page, while the handleForm function handles POST requests to the form submission page. If the request is not a POST request, the function sends a 405 "Method Not Allowed" response. If the request is a POST request, the function retrieves the values from the form submission and sends a 200 "OK" response with the values as text.
// The handleNotFound function is used to handle requests that cannot be matched to a specific handler. It sends a 404 "File Not Found" response with a message that includes the request URI, method, arguments, and form values.
// The code then calls the server.begin function to start the web server listening on port 80.
// The loop function calls the server.handleClient function to handle any incoming connections to the web server.

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// SSID and password for the Wi-Fi network
#ifndef STASSID
#define STASSID "*****"
#define STAPSK "*****"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
  </head>\
  <body>\
    <h1>Arduino text </h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"hello\" value=\"world\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", postForms);
  digitalWrite(led, 0);
}


void handleForm() {
  if (server.method() != HTTP_POST) {
    digitalWrite(led, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(led, 0);
  } else {
    digitalWrite(led, 1);
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; 
      Serial.println(server.arg(i));
      }
    server.send(200, "text/plain", message);
    digitalWrite(led, 0);
  }
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) { 
    // Serial.println("MDNS responder started"); 
    }

  server.on("/", handleRoot);
  server.on("/postform/", handleForm);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void) {
  server.handleClient();
}