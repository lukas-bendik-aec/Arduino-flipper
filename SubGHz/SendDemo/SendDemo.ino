/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #6  
  mySwitch.enableTransmit(6);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
   mySwitch.setPulseLength(400);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* using decimal code */
  Serial.println("--------");
  Serial.println("Sending ON");
  // Turn ON light signal
  mySwitch.send(4469508, 24);
  delay(1000);  
  
  Serial.println("--------");
  // Turn OFF light signal
  Serial.println("Sending OFF");
  mySwitch.send(4469510, 24);
  delay(1000);  

  delay(5000);
}
