/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "************"
#define STAPSK  "************"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

int ledPin = 2; // GPIO2 of ESP8266
WiFiServer server(80);//Service Port

void setup() {
Serial.begin(115200);
delay(10);

pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
server.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
}

void loop() {
// Check if a client has connected
WiFiClient client = server.available();
if (!client) {
return;
}

// Wait until the client sends some data
Serial.println("new client");
while(!client.available()){
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("?state=ON") != -1) {
digitalWrite(ledPin, HIGH);
value = HIGH;
} 
if (request.indexOf("?state=OFF") != -1){
digitalWrite(ledPin, LOW);
value = LOW;
}

//Set ledPin according to the request
//digitalWrite(ledPin, value);

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  do not forget this one
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.print("<h1>Led pin is now:</h1>");

if(value == HIGH) {
client.print("<h2 style='color:red'>On</h2>");  
} else {
client.print("<h2 style='color:red'>Off</h2>");
}
client.println("<br><br>");
client.println("<!DOCTYPE HTML><html lang='en' dir='ltr'><head><meta charset='utf-8'><title></title></head><body><form action='http://192.168.1.7:80' method='get'><button type='submit' name='state' value='ON'>ON</button><br><br><button type='submit' name='state' value='OFF'>OFF</button><br><br></form></html></body></html>");
//client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
//client.println("</html>");

delay(1);
Serial.println("Client disconnected");
Serial.println("");
}
