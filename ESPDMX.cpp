/**
 * DMXESP - An interface to use ESP8266 as DMX controller
 */


/* ----- LIBRARIES ----- */
#include "ESPDMX.h"
#include <HardwareSerial.h>


static int MaxChannel = 16; //Default to sending first 16 channels
static int TotalMaxChann = 60 //First 60 (more or less) channels can be sent with 128 byte hardware FIFO
static uint8_t DmxSpeed = 250000; //Baud Rate
#define DataFormat SERIAL_8N2
//DMXBuffer contains a copy of all values set by user
volatile uint8_t ValBuffer [60]; //Maximum 32 channels before testing for more.
//More channels = keep 2 buffers for TX?

//First position in DmxBuffer contains packet start information
//DmxBuffer frame info = 1 bit Start + 8 bit value + 2 bit stop + 5 bit MTBF = 16 bits
volatile uint16_t DmxBuffer [61];
static uint8_t dmxStarted = 0;
static uint8_t dmxUpdateOnWrite = 1; //If 1, DMX will be sent when write() is called. if 0, Dmx will be updated when update() is called.
volatile _lastSend; //Last time DMX packet was sent. Recommended MaxTime between packets is 1 sec. Minimum will have to be tested

/*JUST IN CASE constants
static uint8_t HI = 1;
static uint8_t LO = 0;
*/


void DMXESPSerial::dmxBegin() {
  dmxStarted = 1;

  //Set up serial port for DMX communication
  Serial1.begin(DmxSpeed, DataFormat);
  Serial1.write(HI);

  //Initialize Values Buffer
  for (int iR=0; iR < TotalMaxChann; iR++) {
    ValBuffer[iR]=0;
  }
  //Initialize start frame of DmxBuffer
}

void DMXESPSerial::write(uint8_t val, int chan) {
  ValBuffer[chan]
}

//This function makes a
uint16_t DMXESPSerial::makeDmxFrame(uint8_t val, int chan) {

}


//Adds option for DMX to be updated automatically
void DMXESPSerial::lastDMXSend(uint8_t val) {
  long now = millis();
  return (now - _lastSend);
}
