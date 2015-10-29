
#ifndef ESPDMX_h
#define ESPDMX_h

#include <inttypes.h>

/* from ArduinoSerial #include <avr/io.h> Version for ESP? */

// ----- CONSTANTS ------

#define DMX_MAX 512 //Maximum number of channels for a DMX universe


// ---- Methods ----

class DMXESPSerial {
public:
  void maxChannel(int channel); //Set the max channels
  void write(int channel, uint8_t value); //Write a value to a specific channel
  void update();
  void autoUpdate(uint8_t val); //0 = no auto-update, 1 = autoupdate
private:
  void dmxBegin();
  void dmxEnd();
  void dmxWrite();
};

extern DMXESPSerial DmxESP;

#endif
