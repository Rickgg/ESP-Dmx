#include <ESPDMX.h>

DMXESPSerial dmx;

void setup() {
  // put your setup code here, to run once:
  dmx.init();
  delay(200);
  pinMode(2, OUTPUT);
}

void loop() {
  
  /**
      dmx.write(1, 200);
      dmx.write(2, 200);
      dmx.write(3, 200);

      dmx.update();
      delay(1000);

      dmx.write(1, 0);
      dmx.write(2, 0);
      dmx.write(3, 0);

      dmx.update();
      delay(1000);
      */

  digitalWrite(2, LOW);
  delayMicroseconds(88);
  
  digitalWrite(2, HIGH);
  delayMicroseconds(8);

  digitalWrite(2, LOW);
  delayMicroseconds(32);
  digitalWrite(2, HIGH);
  delayMicroseconds(8);

  for (int i = 0; i < 511; i++) {
      digitalWrite(2, LOW);
      delayMicroseconds(4);
      digitalWrite(2, HIGH);
      delayMicroseconds(32);
      digitalWrite(2, LOW);
      digitalWrite(2, HIGH);
      delayMicroseconds(8);
  }
  
  
  digitalWrite(2, HIGH);
  delay(1);
}
