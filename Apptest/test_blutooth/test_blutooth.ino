#include <SoftwareSerial.h>
 
#define TX_PIN      7
#define RX_PIN      6
 
SoftwareSerial bluetooth(RX_PIN, TX_PIN);
int baudRate[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
 
void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  
  Serial.println("Configuring, please wait...");
  
  for (int i = 0 ; i < 9 ; i++) {
     bluetooth.begin(baudRate[i]);
     String cmd = "AT+BAUD4"; 
     bluetooth.flush();
     delay(100);
  }
  
  bluetooth.begin(9600);
  Serial.println("Config done");
  while (!bluetooth) {}
  
  Serial.println("Enter AT commands:");
}
 
void loop() {
  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }
}
