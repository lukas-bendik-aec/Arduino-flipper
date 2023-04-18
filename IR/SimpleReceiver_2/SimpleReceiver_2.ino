#include <IRremote.h>

#define IR_RECEIVE_PIN 4

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    Serial.print("Button pressed ID: ");
    Serial.println(IrReceiver.decodedIRData.command);
  }
}
