#include <OneWire.h>

int PIN=3;
OneWire ibutton (PIN); // I button connected on PIN 3.

byte buffer_read[8];
byte buffer_write[8];

//byte newID[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x0F };
//byte newID[8] = { 0x01, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x64 };

void setup() {
  Serial.begin(115200);
  Serial.println("Hello"); // print new line
}

void loop() {
  // Search for an iButton and assign the value to the buffer_read if found.
  Serial.println("Insert first iButton");
  while (!ibutton.search (buffer_read)) {
    ibutton.reset_search();
    delay(1000);
  }
  // At this point an iButton is found
  Serial.println("1-Wire Device Detected, ID is:");

  for (int x = 0; x < 8; x++) {
    Serial.print(buffer_read[x], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");

  // Check if this is a iButton
  if ( buffer_read[0] != 0x01) {
    Serial.println("Device is not a iButton");
  } else {
    Serial.println("Device is a iButton");
  }

  if ( ibutton.crc8( buffer_read, 7) != buffer_read[7]) {
    Serial.println("CRC is not valid!");
  }

  Serial.println("Reading Complete");
  Serial.println();
  delay(3000);
  Serial.println("Insert second iButton");
  
  while (!ibutton.search (buffer_write)) {
    ibutton.reset_search();
    delay(250);
  }

  int n = memcmp ( buffer_read, buffer_write, sizeof(buffer_read) );
  if (n == 0){
      Serial.println("Same Keys Detected!");  
      return;
  }
  
    Serial.print('\n');
    Serial.print("  Writing iButton ID:\n    ");
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);
    for (byte x = 0; x<8; x++){
      writeByte(buffer_read[x]);
      Serial.print('*');
    }
    Serial.print('\n');
    ibutton.reset();

  
  Serial.println("Done.\n\n");
  Serial.println();
  delay(3000);
}

int writeByte(byte data){
  int data_bit;
  for(data_bit=0; data_bit<8; data_bit++){
    if (data & 1){
      digitalWrite(PIN, LOW); pinMode(PIN, OUTPUT);
      delayMicroseconds(60);
      pinMode(PIN, INPUT); digitalWrite(PIN, HIGH);
      delay(10);
    } else {
      digitalWrite(PIN, LOW); pinMode(PIN, OUTPUT);
      pinMode(PIN, INPUT); digitalWrite(PIN, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}
