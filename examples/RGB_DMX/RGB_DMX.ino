/* This example joins two useful libraries for ESP8266 WiFi Module
 * Websockets library for interfacing from either web interface or
 * some external application and DMX library for controlling 
 * devices on a DMX bus.
 * 
 * Install these libraries:
 * https://github.com/Links2004/arduinoWebSockets
 * https://github.com/Rickgg/ESP-Dmx
 * 
 * Web interface allow one to set the channel and control an RGB
 * light, assuming channels for colors follow one after eachother
 * 
 * How to use:
 * 1) Change WiFi settings accordingly to connect to your network
 * 2) Connect an RS485 driver chip to GPIO02( D4 on Nodemcu 1.0)
 * 3) Check out the serial output for IP or 
 * visit http://rgbdmx.local if your device supports mDNS and 
 * you are in the same local network

Copyright Institute IRNAS Rače 2016 - info@irnas.eu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


#include <Arduino.h>
#include <ESPDMX.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// WiFi network settings
const char* ssid     = "SSID";
const char* password = "password";

DMXESPSerial dmx;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Client connected!");
            // send message to client
            webSocket.sendTXT(num, "Connected");
            break;
        case WStype_TEXT: 
            // # is the start for this data       
            if(payload[0] == '#') {
                //data received is comma separated, thats why we do pEnd+1 to start next value
                char * pEnd;
                uint32_t address = strtol ((const char *) &payload[1],&pEnd,16);
                uint32_t red = strtol ((const char *) pEnd+1,&pEnd,16);
                uint32_t green = strtol ((const char *) pEnd+1,&pEnd,16);
                uint32_t blue = strtol ((const char *) pEnd+1,&pEnd,16);
                //write to DMX bus
                dmx.write(address, red);
                dmx.write(address+1, green);
                dmx.write(address+2, blue);
                dmx.update();
            }
            break;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    // connect to WiFi 
    WiFi.hostname("rgbdmx");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    // wait for connection to be established
    while(WiFi.waitForConnectResult() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      Serial.println("WiFi connection failed, retrying.");
      delay(500);
    }

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("rgbdmx")) {
        Serial.println("MDNS responder started");
    }

    // handle index
    server.on("/", []() {
        // send index.html
        server.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);}; connection.onmessage = function (e) {  console.log('Server: ', e.data);}; id_array= new Array(\"addr\",\"r\",\"g\",\"b\"); values=new Array(id_array.length);  function prepareVar(id,position) {  for (i = 0; i < id_array.length; i++) {   var a = parseInt(document.getElementById(id_array[i]).value).toString(16);   if(a.length < 2) { a = '0' + a; }   values[i]=a; } sendVars(); }    function sendVars() { var data = '#'+values; console.log('Data: ' + data); connection.send(data); }  </script></head><body>LED Control:<br/><br/> <form> Starting address: <input id=\"addr\" type=\"number\" placeholder=\"1\" min=\"0\" max=\"512\" step=\"1\" onchange=\"prepareVar('addr',0);;\" /><br/> R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('r',1);\" oninput=\"this.form.r_in.value=this.value\" /><input id=\"r_in\" type=\"number\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('r',1);\" oninput=\"this.form.r.value=this.value\" /><br/> G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('g',2);\" oninput=\"this.form.g_in.value=this.value\" /><input id=\"g_in\" type=\"number\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('g',2);\" oninput=\"this.form.g.value=this.value\" /><br/> B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('b',3);\" oninput=\"this.form.b_in.value=this.value\" /><input id=\"b_in\" type=\"number\" min=\"0\" max=\"255\" step=\"1\" onchange=\"prepareVar('b',3);\" oninput=\"this.form.b.value=this.value\" /><br/> </form></body></html> ");
    });

    server.begin();
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    dmx.init(512); // initialize with bus length
 
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}

void loop() {
    webSocket.loop();
    server.handleClient();
}
