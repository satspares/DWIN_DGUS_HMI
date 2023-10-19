#include <Arduino.h>
#include <DWIN_Arduino.h>
//https://github.com/satspares/DWIN_DGUS_HMI
//doc: T5L_DGUSII-Application-Development-Guide-V2.8-0901.pdf from my github
// Settings as page  108
#define myDebug
// Soft Serial pins 2 and 3 uno or nano
DWIN hmi(2, 3, 115200);
void onHMIEvent(String address, int lastBytes, String message, String response);

void setup()
{
  Serial.begin(9600);
 // register callback
  hmi.hmiCallBack(onHMIEvent);
  // set our starting page
  hmi.setPage(0);
  // fill up some data in the vp's
  hmi.setVPByte(0x5610,0x10);
  hmi.setVPByte(0x5620,0x20);
  hmi.setVPByte(0x5630,0x30);
  delay(3000);
  #ifdef myDebug // serial test should return 10 hex
  Serial.println(hmi.readVPByte(0x5610),HEX);
  #endif
}
void loop()
{
hmi.listen();
}

void onHMIEvent(String address, int lastBytes, String message, String response)
{
#ifdef myDebug
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastBytes, HEX) + " | M : " + message + " | R : " + response + " ]");
#endif
  if (address == "5600"){
    switch (lastBytes) {
      case 0x10:
      Serial.println("First Press");
    break;
      case 0x20:
      Serial.println("Pressing Held");
    break;
      case 0x30:
      Serial.println("Press Release");
    break;
  }

}

}
