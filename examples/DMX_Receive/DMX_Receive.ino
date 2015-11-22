// - - - - -
// ESPDMX - A Arduino library for sending and receiving DMX using the builtin serial hardware port.
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com> (edited by Marcel Seerig)
// This work is licensed under a GNU style license.
//
// Last change: Marcel Seerig <https://github.com/mseerig>
//
// Documentation and samples are available at https://github.com/Rickgg/ESP-Dmx
// - - - - -

#include <ESPDMX.h>

DMXESPSerial dmx;

void setup() {
  Serial.begin(9600);
  dmx.init();               // initialization
  delay(200);               // wait a while (not necessary)
}

void loop() {
  dmx.update();             // update the DMX bus
  int data = dmx.read(1);   // data from channel 1
  Serial.println(data);     // print it out
}
