/**
 * DMXESP - An interface to use ESP8266 as DMX controller
 */

/* ----- LIBRARIES ----- */
#include <Arduino.h>
#include "ESPDMX.h"
#include "HardwareSerial.h"

extern "C" {
  #include "eagle_soc.h"
}

#define dmxMaxChannel 512
#define PIN2 PERIPHS_IO_MUX_GPIO2_U
#define GPIO2 FUNC_GPIO2
#define UART1 FUNC_U1TXD_BK

//#define UART_TX_FIFO_SIZE 0x01

static int MaxChannel = 16; //Default to sending first 16 channels
#define DmxSpeed 250000
#define DataFormat SERIAL_8N2

#define DMXControlPin 0

static uint8_t dmxStarted = 0;

volatile int dmxChannel = 0; //Next channel byte to be sent
volatile int dmxMaxChan = 32; //Last channel used for sending

//DMX value array. Entry 0 will hold startbyte
volatile uint8_t dmxData[dmxMaxChannel + 1];

DMXESPSerial DMXSerial;

void DMXESPSerial::init() {
  //Initialize Values Buffer
  for (int iR=0; iR < dmxMaxChannel; iR++) {
    dmxData[iR] = 0;
  }
  Serial1.begin(DmxSpeed, DataFormat);
  PIN_FUNC_SELECT(PIN2, GPIO2);
  GPOC = (1 << 2);
}

void DMXESPSerial::maxChannel(int Channel) {
  if (dmxStarted == 0) init();
  if (Channel < 1) Channel = 1;
  if (Channel > dmxMaxChannel) return;
  dmxMaxChan = Channel;
}

uint8_t DMXESPSerial::read(int Channel) {
  if (dmxStarted == 0) init();
  if (Channel < 1) Channel = 1;
  if (Channel > dmxMaxChannel) Channel = dmxMaxChannel;
  return(dmxData[Channel]);
}

void DMXESPSerial::write(int Channel, uint8_t value) {
  if (dmxStarted == 0) init();
  if (Channel < 1) Channel = 1;
  if (Channel > dmxMaxChannel) Channel = dmxMaxChannel;
  if (value < 0) value = 0;
  if (value > 255) value = 255;

  dmxData[Channel] = value;
}

void DMXESPSerial::update() {
  //Send break
  GPOS = (1 << 2);
  delayMicroseconds(120);
  GPOC = (1 << 2);
  delayMicroseconds(12);

  PIN_FUNC_SELECT(PIN2, UART1);
  Serial1.write(0);
  Serial1.flush();
  for (int iR = 0; iR < dmxMaxChan; iR++){
    Serial1.write(dmxData[iR]);
    Serial1.flush();
  }
  delay(1);
  PIN_FUNC_SELECT(PIN2, GPIO2);
  GPOC = (1 << 2);
}
