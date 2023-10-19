#include <Arduino.h>
#include <DWIN_Arduino.h>
//Tested with Nucleo-F411RE

  HardwareSerial Serial1(PA10, PA9);

  #define DGUS_BAUD 115200
  #define DGUS_SERIAL Serial1
  DWIN hmi(DGUS_SERIAL);

  void onHMIEvent(String address, int lastByte, String message, String response);
  uint16_t readVP(uint16_t vpAddress);


void setup() {
   Serial.begin(9600);
   DGUS_SERIAL.begin(DGUS_BAUD);
   hmi.echoEnabled(false);                    
   hmi.hmiCallBack(onHMIEvent); 

   Serial.println("DWIN HMI ~ STM32 Hello World");
   Serial.print("DWIN Hardware Version ");
   Serial.println(hmi.getHWVersion());

   hmi.setPage(0);
   hmi.setBrightness(0x20);       // see the docs 0x82 LED_Config
   hmi.setVPWord(0x1000,0x55AA);  // put something in VP 0x1000
}

void loop() {
  hmi.listen();
  delay(1000);
  // Read VP 0x1000
  Serial.println(readVP(0x1000));
}

void onHMIEvent(String address, int lastByte, String message, String response)
{

  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");

}

// Read word from VP address 
uint16_t readVP(uint16_t vpAddress)
{
    return (hmi.readVPByte(vpAddress,1) << 8) + hmi.readVPByte(vpAddress) ;
}
