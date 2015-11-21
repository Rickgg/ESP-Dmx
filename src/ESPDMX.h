
#ifndef ESPDMX_h
#define ESPDMX_h

#include <inttypes.h>

// ---- Methods ----

class DMXESPSerial {
public:
  void init();
  void maxChannel(int channel); //Set the max channels
  uint8_t read(int Channel);
  void write(int channel, uint8_t value); //Write a value to a specific channel
  void update();
};

#endif
