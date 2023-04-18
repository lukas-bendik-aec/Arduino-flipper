#include <OneWire.h>

OneWire ibutton (3); // I button connected on PIN 2.

byte buffer[8];

void setup(){
  Serial.begin(115200);
  Serial.println("Hello"); // print new line
}

void loop(){
  // Search for an iButton and assign the value to the buffer if found.
  if (!ibutton.search (buffer)){
     ibutton.reset_search();
     delay(250);
     return;
  }
  // At this point an iButton is found
  Serial.println("1-Wire Device Detected, ID is:");

  for (int x = 0; x<8; x++){
    Serial.print(buffer[x],HEX);
    Serial.print(" ");
  }
  Serial.print("\n");

  // Check if this is a iButton
  if ( buffer[0] != 0x01) {
    Serial.println("Device is not a iButton");
  } else {
    Serial.println("Device is a iButton");
  }

  if ( ibutton.crc8( buffer, 7) != buffer[7]) {
      Serial.println("CRC is not valid!");
  }
  Serial.println("Done.\n\n");  
}
