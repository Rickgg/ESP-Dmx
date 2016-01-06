/*
  Example code to create a Streaming ACN (E1.31) node which receives sACN packets and transmits DMX

  created 5 January 2016
  by Ben Franske <ben@franskeconsulting.com>

  This example code is in the public domain.

 Circuit:
  ESP8266 UART1 Tx (check the exact pinout of your ESP8266 module, UART1 Tx is GPIO2 which is D4 on my nodeMCU breakout board)
  connected to data input on RS485 driver chip (example chip is TI 75176BP) wired for constant transmit and power
  DMX lights connected to A & B of RS485 driver chip. Note, this is a simple unisolated driver circuit not suitable for production use.

 Use:
  Connect a wireless device capable of sending unicast sACN data (such as a tablet or PC runnign suitable software)
  to the SSID specified below. Send data to the apIP address specified below on the standard sACN UDP port number.
  Data will be converted to DMX format and output on UART1 for input to RS485 driver chip. Example open source PC
  software which supports sACN transmitting is Q Light Controller Plus <http://www.qlcplus.org/>
 
 Requires:
  ESP8266 Core for Arduino: <https://github.com/esp8266/Arduino/>
  ESP-Dmx library: <https://github.com/Rickgg/ESP-Dmx>
  E1.31 library: <https://github.com/forkineye/E131>

 *  This program is provided free for you to use in any way that you wish,
 *  subject to the laws and regulations where you are using it.  Due diligence
 *  is strongly suggested before using this code.  Please give credit where due.
 *
 *  The Author makes no warranty of any kind, express or implied, with regard
 *  to this program or the documentation contained in this document.  The
 *  Author shall not be liable in any event for incidental or consequential
 *  damages in connection with, or arising out of, the furnishing, performance
 *  or use of these programs.

*/

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <E131.h>
#include <ESPDMX.h>

const char ssid[] = "Sample sACN Node";         /* Replace with your SSID */
const char passphrase[] = "";   /* Replace with your WPA2 passphrase 8-63 character, null for none */

IPAddress apIP(192, 168, 1, 1);
IPAddress apSNM(255, 255, 255, 0);

E131 e131;
DMXESPSerial dmx;

void setup() {
  delay(1000);
//  Serial.begin(115200);
//  Serial.println();
//  Serial.print("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, apSNM);
  WiFi.softAP(ssid, passphrase);

  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
  e131.begin(E131_UNICAST, 1);
  dmx.init(); // only initializes for 32 channels by default
}

void loop() {
    uint16_t numChannels = e131.parsePacket();
    if (numChannels) {
      for (int thisChannel = 0; thisChannel <= numChannels; thisChannel++) {
        dmx.write(thisChannel+1, e131.data[thisChannel]);
        }
      }
    dmx.update();
}

