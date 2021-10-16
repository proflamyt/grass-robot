#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
// Replace with your network credentials
const char* ssid = "Olamide";
const char* password = "olamideyyyy";


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
SoftwareSerial s(D2, D1);  

// Auxiliar variables to store the current output state


// Assign output variables to GPIO pins


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
String output26State = "on";
char level =0;
char moisture = 0;

void setup() {
  Serial.begin(115200);
  s.begin(9600);

if (!s) { // If the object did not initialize, then its configuration is invalid
  Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
  while (1) { // Don't continue with invalid configuration
    delay (1000);
  }
} 

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  server.begin();
}



/*

  int soilSensor(){
  int ola = analogRead(soilPin);
  }
*/

void loop() {
 
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            

            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {

              s.write(1);
              output26State = "off";


            } else if (header.indexOf("GET /26/off") >= 0) {
              s.write(2);
              output26State = "on";

            }



            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            while (s.available() > 0) {
              level = s.read();
            }
            client.println("<h1> Water Level :" + level);

             while (s.available() > 0) {
              moisture = s.read();
            }
            client.println("<h1> Water Moisture :" +moisture);
            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output26State == "off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }


            // If the output27State is off, it displays the ON button

        

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }



}
