
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "ESPDMX.h"

const char *ssid = "DMXServer";
const char *password = "Enhance";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);


Ticker coloring;
DMXESPSerial DMX;

String responseHTML = "<h1>WELCOME!</h1><h2>You have entered a new level of experience!</h2>";

uint8_t Red[]   =   {255, 255, 0,   0,  176,173,135,135,0  ,30 ,65 ,25 ,0  ,0  ,60 ,46 ,34 ,0  ,107,245,250,238,255,255,255,255,184,255,178,255,255,208,218,128};
uint8_t Green[] = {255, 0,   255, 0,  224,216,206,206,191,144,105,25 ,250,255,179,139,139,128,142,245,250,232,255,215,165,140,134,160,34 ,0  ,20 ,32 ,112,0};
uint8_t Blue[]  =  {255, 0,   0,   255,230,230,230,235,255,255,225,112,154,127,113,87 ,34 ,0  ,35 ,220,210,170,0  ,0  ,0  ,0  ,11 ,122, 34,255,147,144,214,128};
bool TickerAttached = false;

void colorMe() {
  int Color = random(0, 33);
  DMX.write(1, Red[Color]);
  DMX.write(2, Green[Color]);
  DMX.write(3, Blue[Color]);
}

void blackout() {
  if (TickerAttached) coloring.detach();
  server.send(200, "text/html", responseHTML + "<h1> Entered DMX blackout</h1>");
  DMX.write(1, 0);
  DMX.write(2, 0);
  DMX.write(3, 0);
}

void randomize() {
  server.send(200, "text/html", responseHTML + "<h1> Entered Random</h1>");
  TickerAttached = true;
  coloring.attach(.5, colorMe);
}

void White() {
  if (TickerAttached) coloring.detach();
  server.send(200, "text/html", responseHTML + "<h1>Entered White</h1>");
  DMX.write(1, 255);
  DMX.write(2, 255);
  DMX.write(3, 255);
}

void green() {
  if (TickerAttached) coloring.detach();
  server.send(200, "text/html", responseHTML + "<h1>Entered Green</h1>");
  DMX.write(1, 0);
  DMX.write(2, 255);
  DMX.write(3, 0);
}

void red() {
  if (TickerAttached) coloring.detach();
  server.send(200, "text/html", responseHTML + "<h1>Entered red</h1>");
  DMX.write(1, 255);
  DMX.write(2, 0);
  DMX.write(3, 0);
}

void blue() {
  if (TickerAttached) coloring.detach();
  server.send(200, "text/html", responseHTML + "<h1>Entered blue</h1>");
  DMX.write(1, 0);
  DMX.write(2, 0);
  DMX.write(3, 255);
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("DMX Server example");

  dnsServer.start(DNS_PORT, "dmxserver.com", apIP);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address:");
  Serial.println(myIP);
  server.on("/", blackout);
  server.on("/white", White);
  server.on("/green", green);
  server.on("/red", red);
  server.on("/blue", blue);
  server.on("/random", randomize);
  server.begin();
  Serial.println("HTTP server started");

  DMX.init();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  DMX.update();
}
