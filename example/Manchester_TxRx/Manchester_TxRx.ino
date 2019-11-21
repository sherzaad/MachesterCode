#include <Manchester.h>

Manchester mCode(4, 5); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the clock rate for the Manchester code transmission
  mCode.begin(4800);
  
  Serial.println("Ready!");
}

void loop() { // run over and over 
  if (mCode.available()) {
    Serial.print("RX: ");
    Serial.println(mCode.read(),HEX);
  }
  
  if (Serial.available()) {
    byte d = Serial.read() ;
    Serial.print("TX: ");
	Serial.println(d,HEX);
    mCode.write(d);
  }
}
