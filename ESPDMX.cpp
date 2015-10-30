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

//First position in DmxBuffer contains packet start information
//DmxBuffer frame info = 1 bit Start + 8 bit value + 2 bit stop + 5 bit MTBF = 16 bits
volatile uint16_t DmxBuffer [60];
//More channels = keep 2 buffers for TX?

static uint8_t _dmxStarted = 0;
static uint8_t _dmxUpdateOnWrite = 1; //If 1, DMX will be sent when write() is called. if 0, Dmx will be updated when update() is called.
volatile _lastSend; //Last time DMX packet was sent. Recommended MaxTime between packets is 1 sec. Minimum will have to be tested

static uint8_t _HI = B11111111;


static uint16_t packetStart1 = 49152; //half start packet :O
static uint16_t packetStart2 = 7; //second half of start packet
static uint16_t startCode = 127;
//DMX workflow = packetStart1 + packetStart2 + startCode + 1-512 frames + HI

void DMXESPSerial::dmxBegin() {
  dmxStarted = 1;

  //Set up serial port for DMX communication
  Serial1.begin(DmxSpeed, DataFormat);
  Serial1.write(HI);

  //Initialize Values Buffer
  for (int iR=0; iR < TotalMaxChann; iR++) {
    write(0, iR);
  }
  //Initialize start frame of DmxBuffer
}

//This function makes a DmxBuffer frame = 1 bit Start + 8 bit value + 2 bit stop + 5 bit MTBF = 16 bits
uint16_t DMXESPSerial::write(uint16_t value, int chan) {
  if (!= _dmxStarted) dmxBegin();

  uint16_t frame = B11111111;
  value = value << 7;
  frame = frame | val;
  DmxBuffer[chan] = frame;
}

void DMXESPSerial::sendInfo() {
  if (!= _dmxStarted) dmxBegin();
  Serial1.write(packetStart1);
  Serial1.write(packetStart2);
  Serial1.write(startCode);
  for (int iR = 0; iR < MaxChannel; iR++)
    Serial1.write(DmxBuffer[iR]);
  Serial.write(HI);
}


//Adds option for DMX to be updated automatically
void DMXESPSerial::lastDMXSend(uint8_t val) {
  long now = millis();
  return (now - _lastSend);
}
