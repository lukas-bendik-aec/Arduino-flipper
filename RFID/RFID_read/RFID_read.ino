#include <SoftwareSerial.h>

const int BUFFER_SIZE = 14; 
const int DATA_SIZE = 10; 
const int DATA_VERSION_SIZE = 2; 
const int DATA_TAG_SIZE = 8;
const int CHECKSUM_SIZE = 2;

SoftwareSerial ssrfid = SoftwareSerial(5,4); 

uint8_t buffer[BUFFER_SIZE];
int buffer_index = 0;

void setup() {
 Serial.begin(9600); 
 
 ssrfid.begin(9600);
 ssrfid.listen(); 
 
 Serial.println("Initialization Done!");
}

void loop() {
  if (ssrfid.available() > 0){
    bool call_extract_tag = false;
    
    int ssvalue = ssrfid.read(); 
    if (ssvalue == -1) { 
      return;
    }

    if (ssvalue == 2) { 
      buffer_index = 0;
    } else if (ssvalue == 3) {       
      call_extract_tag = true; 
    }

    if (buffer_index >= BUFFER_SIZE) { 
      Serial.println("Error: Buffer overflow detected! ");
      return;
    }
    
    buffer[buffer_index++] = ssvalue; 

    if (call_extract_tag == true) {
      if (buffer_index == BUFFER_SIZE) {
        unsigned tag = extract_tag();
      } else { 
        buffer_index = 0;
        return;
      }
    }    
  }    
}

unsigned extract_tag() {
    uint8_t msg_head = buffer[0];
    uint8_t *msg_data = buffer + 1; 
    uint8_t *msg_data_version = msg_data;
    uint8_t *msg_data_tag = msg_data + 2;
    uint8_t *msg_checksum = buffer + 11;
    uint8_t msg_tail = buffer[13];
    Serial.println("--------");
    Serial.print("Message-Head: ");
    Serial.println(msg_head);

    Serial.println("Message-Data (HEX): ");
    for (int i = 0; i < DATA_VERSION_SIZE; ++i) {
      Serial.print(char(msg_data_version[i]));
    }
    Serial.println(" (version)");
    for (int i = 0; i < DATA_TAG_SIZE; ++i) {
      Serial.print(char(msg_data_tag[i]));
    }
    Serial.println(" (tag)");

    Serial.print("Message-Checksum (HEX): ");
    for (int i = 0; i < CHECKSUM_SIZE; ++i) {
      Serial.print(char(msg_checksum[i]));
    }
    Serial.println("");

    Serial.print("Message-Tail: ");
    Serial.println(msg_tail);

    Serial.println("--");

    long tag = hexstr_to_value(msg_data_tag, DATA_TAG_SIZE);
    Serial.print("Extracted Tag: ");
    Serial.println(tag);

    long checksum = 0;
    for (int i = 0; i < DATA_SIZE; i+= CHECKSUM_SIZE) {
      long val = hexstr_to_value(msg_data + i, CHECKSUM_SIZE);
      checksum ^= val;
    }
    Serial.print("Extracted Checksum (HEX): ");
    Serial.print(checksum, HEX);
    if (checksum == hexstr_to_value(msg_checksum, CHECKSUM_SIZE)) { 
      Serial.print(" (OK)"); 
    } else {
      Serial.print(" (NOT OK)"); 
    }

    Serial.println("");
    Serial.println("--------");

    return tag;

}
long hexstr_to_value(char *str, unsigned int length) { 
  char* copy = malloc((sizeof(char) * length) + 1); 
  memcpy(copy, str, sizeof(char) * length);
  copy[length] = '\0'; 
  long value = strtol(copy, NULL, 16);  
  free(copy); 
  return value;
}
