
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "ESPDMX.h"

const char *ssid = "DMXServer";
const char *password = "Enhance";

Ticker coloring;
DMXESPSerial DMX;
ESP8266WebServer server(80);

uint8_t Red[] = {255, 255, 0, 0, 176, 135, 0, 0, 255 184};
uint8_t Green[] = {255, 0, 255, 0, 224, 206, 0 250, 255, 134};
uint8_t Blue[] = {255, 0, 0, 255,230, 191, 0, 154, 0, 11};
bool TickerAttached = false;

void colorMe() {
  int Color = random(0, 9);
  DMX.write(1, Red[Color]);
  DMX.write(2, Green[Color]);
  DMX.write(3, Blue[Color]);
}

void blackout() {
  if (TickerAttached) coloring.detach();
  server.send(0, "text/html", "<h1> Entered DMX blackout</h1>");
  DMX.write(1, 0);
  DMX.write(2, 0);
  DMX.write(3, 0);
}

void rainbow() {
  server.send(0, "text/html", "<h1> Entered Rainbow</h1>");
}

void random() {
  server.send(0, "text/html", "<h1> Entered Random</h1>");
  TickerAttached = true;
  coloring.attach(.5, colorMe);
}

void White() {
  if (TickerAttached) coloring.detach();
  server.send(0, "text/html", "<h1>Entered White</h1>");
  DMX.write(1, 255);
  DMX.write(2, 255);
  DMX.write(3, 255);
}

void green() {
  if (TickerAttached) coloring.detach();
  server.send(0, "text/html", "<h1>Entered Green</h1>");
  DMX.write(1, 0);
  DMX.write(2, 255);
  DMX.write(3, 0);
}

void red() {
  if (TickerAttached) coloring.detach();
  server.send(0, "text/html", "<h1>Entered red</h1>");
  DMX.write(1, 255);
  DMX.write(2, 0);
  DMX.write(3, 0);
}

void blue() {
  if (TickerAttached) coloring.detach();
  server.send(0, "text/html", "<h1>Entered blue</h1>");
  DMX.write(1, 0);
  DMX.write(2, 0);
  DMX.write(3, 255);
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address:");
  Serial.println(myIP);
  server.on("/", blackout);
  server.on("/white", White);
  server.on("/green", green);
  server.on("/red", red);
  server.on("/blue", blue);
  server.begin();
  Serial.println("HTTP server started");

  DMX.init();
}

void loop() {
  server.handleClient();
  DMX.update();
}
