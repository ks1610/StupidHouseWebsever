/*

// Load Wi-Fi library
#include <WiFi.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char* ssid = "Hien-Khanh-Ha_plus";
const char* password = "khanhha@123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String output25State = "off";
String output33State = "off";
String output32State = "off";
String output34State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
const int output25 = 25;
const int output33 = 33;
const int output32 = 32;
const int output34 = 34;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  /* Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }*/
/*  
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(output25, OUTPUT);
  pinMode(output33, OUTPUT);
  pinMode(output32, OUTPUT);
  pinMode(output34, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(output25, LOW);
  digitalWrite(output33, LOW);
  digitalWrite(output32, LOW);
  digitalWrite(output34, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
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
        Serial.write(c);                    // print it out the serial monitor
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
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }else if (header.indexOf("GET /25/on") >= 0) {
                Serial.println("GPIO 25 on");
                output25State = "on";
                digitalWrite(output25, HIGH);
            } else if (header.indexOf("GET /25/off") >= 0) {
                Serial.println("GPIO 25 off");
                output25State = "off";
                digitalWrite(output25, LOW);
            } else if (header.indexOf("GET /33/on") >= 0) {
                Serial.println("GPIO 33 on");
                output33State = "on";
                digitalWrite(output33, HIGH);
            } else if (header.indexOf("GET /33/off") >= 0) {
                Serial.println("GPIO 33 off");
                output33State = "off";
                digitalWrite(output33, LOW);
            } else if (header.indexOf("GET /32/on") >= 0) {
                Serial.println("GPIO 32 on");
                output32State = "on";
                digitalWrite(output32, HIGH);
            } else if (header.indexOf("GET /32/off") >= 0) {
                Serial.println("GPIO 32 off");
                output32State = "off";
                digitalWrite(output32, LOW);
            } else if (header.indexOf("GET /34/on") >= 0) {
                Serial.println("GPIO 34 on");
                output34State = "on";
                digitalWrite(output34, HIGH);
            } else if (header.indexOf("GET /34/off") >= 0) {
                Serial.println("GPIO 34 off");
                output34State = "off";
                digitalWrite(output34, LOW);
            }

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 33px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            // Display current state, and ON/OFF buttons for GPIO 25  
            client.println("<p>GPIO 25 - State " + output25State + "</p>");
            if (output25State == "off") {
                client.println("<p><a href=\"/25/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/25/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 33
            client.println("<p>GPIO 33 - State " + output33State + "</p>");
            if (output33State == "off") {
                client.println("<p><a href=\"/33/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/33/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 32
            client.println("<p>GPIO 32 - State " + output32State + "</p>");
            if (output32State == "off") {
                client.println("<p><a href=\"/32/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/32/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 34
            client.println("<p>GPIO 34 - State " + output34State + "</p>");
            if (output34State == "off") {
                client.println("<p><a href=\"/34/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/34/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
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
*/


// Load Wi-Fi library
#include <WiFi.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char* ssid = "Hien-Khanh-Ha_plus";
const char* password = "khanhha@123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";
String output25State = "off";
String output33State = "off";
String output32State = "off";
String output34State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
const int output25 = 25;
const int output33 = 33;
const int output32 = 32;
const int output34 = 34;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  /* Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }*/
  
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(output25, OUTPUT);
  pinMode(output33, OUTPUT);
  pinMode(output32, OUTPUT);
  pinMode(output34, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(output25, LOW);
  digitalWrite(output33, LOW);
  digitalWrite(output32, LOW);
  digitalWrite(output34, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
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
        Serial.write(c);                    // print it out the serial monitor
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
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }else if (header.indexOf("GET /25/on") >= 0) {
                Serial.println("GPIO 25 on");
                output25State = "on";
                digitalWrite(output25, HIGH);
            } else if (header.indexOf("GET /25/off") >= 0) {
                Serial.println("GPIO 25 off");
                output25State = "off";
                digitalWrite(output25, LOW);
            } else if (header.indexOf("GET /33/on") >= 0) {
                Serial.println("GPIO 33 on");
                output33State = "on";
                digitalWrite(output33, HIGH);
            } else if (header.indexOf("GET /33/off") >= 0) {
                Serial.println("GPIO 33 off");
                output33State = "off";
                digitalWrite(output33, LOW);
            } else if (header.indexOf("GET /32/on") >= 0) {
                Serial.println("GPIO 32 on");
                output32State = "on";
                digitalWrite(output32, HIGH);
            } else if (header.indexOf("GET /32/off") >= 0) {
                Serial.println("GPIO 32 off");
                output32State = "off";
                digitalWrite(output32, LOW);
            } else if (header.indexOf("GET /34/on") >= 0) {
                Serial.println("GPIO 34 on");
                output34State = "on";
                digitalWrite(output34, HIGH);
            } else if (header.indexOf("GET /34/off") >= 0) {
                Serial.println("GPIO 34 off");
                output34State = "off";
                digitalWrite(output34, LOW);
            }

            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("button {background-color: #9cc9cb; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 33px; margin: 2px; cursor: pointer;width: 180px;height: 180px;text-align: center;}");
            client.println(".button_26{background-image: url(\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSNiv_i0z7SUFMXKZYbYMlB_kNrSsj1VvfywA6Y0Y-row&s\"); background-size: cover;background-position: center top;}");
            client.println(".button_27{background-image: url(\"https://t3.ftcdn.net/jpg/02/08/25/66/360_F_208256678_37zjgDuOcgPgmV7mpRVJr1hKQp6te9dY.webp\");background-size: cover;background-position: center top;}");
            client.println(".button_25, .button_33, .button_32{background-image: url(\"https://www.shutterstock.com/image-vector/double-bed-lamp-showing-bedroom-260nw-1190868772.jpg\");background-size: cover;background-position: center top;}");
            client.println(".button_34{background-image: url(\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRpWN_GZLJ6HYcGa8XOeMXp_6T8i-0iekMcf-NwEtx6kQ&s\");background-size: cover;background-position: center top;}");
            client.println(".gpio-container-1, .gpio-container-2, .gpio-container-3 {display: flex; margin: 20px;flex-direction: row; justify-content: center;}");
            client.println(".gpio{ flex: 1; margin: 0 10px;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Welcome</h1>");
            

            /*
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<div class=\"gpio-container-1\"><div><p>Livingroom - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p></div>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<div><p>Kitchen - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p></div></div>");
            }
            
            /*
            // Display current state, and ON/OFF buttons for GPIO 25  
            client.println("<p>GPIO 25 - State " + output25State + "</p>");
            if (output25State == "off") {
                client.println("<p><a href=\"/25/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/25/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 33
            client.println("<p>GPIO 33 - State " + output33State + "</p>");
            if (output33State == "off") {
                client.println("<p><a href=\"/33/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/33/off\"><button class=\"button button2\">OFF</button></a></p>");
            }*/

            /*
            // Display current state, and ON/OFF buttons for GPIO 25  
            client.println("<div class=\"gpio-container-2\"><div><p>Bedroom 1 - State " + output25State + "</p>");
            // If the output25State is off, it displays the ON button       
            if (output25State=="off") {
                client.println("<p><a href=\"/25/on\"><button class=\"button\">Bed Room</button></a></p>");
            } else {
                client.println("<p><a href=\"/25/off\"><button class=\"button button2\">Bed Room</button></a></p></div>");
            } 

            // Display current state, and ON/OFF buttons for GPIO 33  
            client.println("<div><p>Bedroom 2 - State " + output33State + "</p>");
            // If the output33State is off, it displays the ON button       
            if (output33State=="off") {
                client.println("<p><a href=\"/33/on\"><button class=\"button\">Bed Room</button></a></p>");
            } else {
                client.println("<p><a href=\"/33/off\"><button class=\"button button2\">Bed Room</button></a></p></div></div>");
            }*/

            /*
            // Display current state, and ON/OFF buttons for GPIO 32
            client.println("<p>GPIO 32 - State " + output32State + "</p>");
            if (output32State == "off") {
                client.println("<p><a href=\"/32/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/32/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 34
            client.println("<p>GPIO 34 - State " + output34State + "</p>");
            if (output34State == "off") {
                client.println("<p><a href=\"/34/on\"><button class=\"button\">ON</button></a></p>");
            } else {
                client.println("<p><a href=\"/34/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            */
           /*
            // Display current state, and ON/OFF buttons for GPIO 32  
            client.println("<div class=\"gpio-container-3\"><div><p>Bedroom 3 - State " + output32State + "</p>");
            // If the output32State is off, it displays the ON button       
                if (output32State=="off") {
            client.println("<p><a href=\"/32/on\"><button class=\"button\">Bed Room</button></a></p>");
            } else {
                client.println("<p><a href=\"/32/off\"><button class=\"button button2\">Bed Room</button></a></p></div>");
            } 

            // Display current state, and ON/OFF buttons for GPIO 34  
            client.println("<div><p>Garden - State " + output34State + "</p>");
            // If the output34State is off, it displays the ON button       
            if (output34State=="off") {
                client.println("<p><a href=\"/34/on\"><button class=\"button\">Garden</button></a></p>");
            } else {
                client.println("<p><a href=\"/34/off\"><button class=\"button button2\">Garden</button></a></p></div></div>");
            }*/

            // Display current state, and ON/OFF buttons for GPIO 26 and 27 in the same line
            client.println("<div class=\"gpio-container-1\"><div class=\"gpio\"><p>Livingroom - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button_26\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button_26 button2\"></button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("</div><div class=\"gpio\"><p>Kitchen - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button_27\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button_27 button2\"></button></a></p>");
            }
            client.println("</div></div>");

            // Display current state, and ON/OFF buttons for GPIO 25 and 33 in the same line
            client.println("<div class=\"gpio-container-2\"><div class=\"gpio\"><p>Bedroom 1 - State " + output25State + "</p>");
            // If the output25State is off, it displays the ON button       
            if (output25State=="off") {
              client.println("<p><a href=\"/25/on\"><button class=\"button_25\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/25/off\"><button class=\"button_25 button2\"></button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 33
            client.println("</div><div class=\"gpio\"><p>Bedroom 2 - State " + output33State + "</p>");
            // If the output33State is off, it displays the ON button       
            if (output33State=="off") {
              client.println("<p><a href=\"/33/on\"><button class=\"button_33\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/33/off\"><button class=\"button_33 button2\"></button></a></p>");
            }
            client.println("</div></div>");

            // Display current state, and ON/OFF buttons for GPIO 32 and 34 in the same line
            client.println("<div class=\"gpio-container-3\"><div class=\"gpio\"><p>Bedroom 3 - State " + output32State + "</p>");
            // If the output32State is off, it displays the ON button       
            if (output32State=="off") {
              client.println("<p><a href=\"/32/on\"><button class=\"button_32\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/32/off\"><button class=\"button_32 button2\"></button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 34
            client.println("</div><div class=\"gpio\"><p>Garden - State " + output34State + "</p>");
            // If the output34State is off, it displays the ON button       
            if (output34State=="off") {
              client.println("<p><a href=\"/34/on\"><button class=\"button_34\"></button></a></p>");
            } else {
              client.println("<p><a href=\"/34/off\"><button class=\"button_34 button2\"></button></a></p>");
            }
            client.println("</div></div>");


            client.println("</body></html>");
            
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
