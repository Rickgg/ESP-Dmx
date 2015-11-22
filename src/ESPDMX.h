// - - - - -
// ESPDMX - A Arduino library for sending and receiving DMX using the builtin serial hardware port.
// ESPDMX.cpp: Library implementation file
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license. 
//
// Last change: Marcel Seerig <https://github.com/mseerig>
//
// Documentation and samples are available at https://github.com/Rickgg/ESP-Dmx
// - - - - -

#ifndef ESPDMX_h
#define ESPDMX_h

#include <inttypes.h>

// ---- Methods ----

class DMXESPSerial {
public:
  void init(); 
  uint8_t read(int Channel);
  void write(int channel, uint8_t value); //Write a value to a specific channel
  void update();
};

#endif
