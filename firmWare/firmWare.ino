#include <WiFi.h>
#include <WebServer.h>

// Setting the Wifi Server Credentials
const char* ssid = "Baraza's_Server";
const char* password = "12345Baraza";

// setting the IP addresses
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 0);
IPAddress subnet(255, 255, 255, 0);

// Declare useful pins and initialize their states to low
int pin1 = 22;
bool pin1Status = HIGH;
int pin2 = 23;
bool pin2Status = HIGH;

// Initialize the Server
WebServer server(80);     // This port Listens the HTTP request and process them accordingly

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);

  //Initialize the ESP32 with the Credentials for it to be a Server
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // HTTP Request handler by the on()
  server.on("/", serverOn);
  server.on("/pin1On", pin1On);
  server.on("/pin1Off", pin1Off);
  server.on("/pin2On", pin2On);
  server.on("/pin2Off", pin2Off);

  server.begin();
  Serial.println("Server Started.!");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if(pin1Status)
    digitalWrite(pin1, HIGH);
  else
    digitalWrite(pin1, LOW);

  if(pin2Status)
    digitalWrite(pin2, HIGH);
  else
    digitalWrite(pin2, LOW);
}


void serverOn() {
  pin1Status = LOW;   pin2Status = LOW;
  Serial.println("Pin1 : OFF || Pin2 : OFF");
  server.send(200, "text/html", sendHTML(pin1Status, pin2Status));
}

void pin1On() {
  pin1Status = HIGH;
  Serial.println("Pin1 : ON");
  server.send(200, "text/html", sendHTML(true, pin2Status));
}

void pin1Off() {
  pin1Status = LOW;
  Serial.println("Pin1 : OFF");
  server.send(200, "text/html", sendHTML(false, pin2Status));
}

void pin2On() {
  pin2Status = HIGH;
  Serial.println("Pin2 : ON");
  server.send(200, "text/html", sendHTML(pin1Status, true));
}

void pin2Off() {
  pin2Status = LOW;
  Serial.println("Pin2 : OFF");
  server.send(200, "text/html", sendHTML(pin1Status, false));
}

String sendHTML(int pin1Status, int pin2Status) {
  String page = "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>ESP32 Server</title> <style> body { margin: 0; padding: 0; height: 100vh; display: grid; grid-template-columns: repeat(3, minmax(100px, auto)); grid-template-rows: repeat(3, minmax(100px, auto)); } #bd { padding-top: 100px; border-radius: 30px; box-shadow: 5px 5px 5px rgb(171, 171, 171); background-color: antiquewhite; grid-row-start: 2; grid-row-end: 3; grid-column-start: 2; grid-column-end: 3; text-align: center; } button { font-weight: bold; background-color: rgb(51, 51, 242); padding: 12px 15px; border-radius: 12px; outline: none; color: aliceblue; text-decoration: none; border: none; margin: 4px; cursor: pointer; transition: .5s; box-shadow: 5px 5px 5px rgb(132, 131, 131); } button.off { background-color: red; transition: .5s; } button:hover { background-color: blue; transform: scale(1.1); transition: .3s; } button.off:hover { background-color: rgb(212, 30, 91); color: black; transition: .5s; } a { text-decoration: none; } span { color: red; }/* MediaQueries */@media only screen and (max-width: 600px) { h1 { font-size: 15px; } h3 { font-size: 13px; } button { padding: 8px 10px; font-size: 10px; } #bd { padding-top: 80px; margin-top: 20px; grid-row-start: 2; grid-row-end: 3; grid-column-start: 1; grid-column-end: 4; text-align: center; margin: 50px; }} </style></head><body> <div id=\"bd\"> <h1>Baraza's <span>BULB</span> Controller</h1> <h3>2 Controllable <span>Bulb's</span></h3> <a href=\"/pin1Off\"> <button> ON </button> </a> <a href=\"/pin2Off\"> <button> ON </button> </a> <br> <a href=\"/pin1On\"> <button class=\"off one\"> OFF </button> </a> <a href=\"/pin2On\"> <button class=\"off two\"> OFF </button> </a> </div></body></html>";
return page;
}



