
/*
 * -----------------------------------------------------------------------------------------
 * DWIN Hello World Sketch   | Author : Tejeet ( tejdwin@gmail.com ) and satspares@gmail.com
 * -----------------------------------------------------------------------------------------
 * This is DWIN HMI Library for Arduino Compatible Boards. More Info about Display Visit
 * Official Site --> https://www.dwin-global.com/
 *
 * Example sketch/program showing how to initialize DWIN Hmi with Arduino Nano_Every
 * In this example we can see on the setup loop we change the page no and set the brightness &
 * Listen to display Events from serial port
 *
 */

#include <Arduino.h>
#include <DWIN_Arduino.h>

#define DGUS_BAUD 115200
// 
DWIN hmi(Serial1, DGUS_BAUD, false);     // the false do not init the port yet

// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response);


void setup()
{
   Serial.begin(9600);
   delay(3000);
   // Start Nano_Every Serial1
   // Also see for extra ports https://emalliab.wordpress.com/2021/12/16/arduino-nano-every-serial-ports/
   hmi.initSerial(Serial1, DGUS_BAUD );
  
   Serial.println("DWIN HMI ~ Hello World Nano_Every");
   Serial.print("DWIN Hardware Version ");
   Serial.println(hmi.getHWVersion());
   Serial.print("DWIN GUI Software Version ");
   Serial.println(hmi.getGUISoftVersion());
   hmi.echoEnabled(false);
   hmi.hmiCallBack(onHMIEvent);
   hmi.setPage(0);  // or your start page
   hmi.setBrightness(0x20); // see the docs 0x82 LED_Config
}

void loop()
{
  // Listen HMI Events
  hmi.listen();
} // End our loop


// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");
  if (address == "1002")
  {
    // Take your custom action call
  }
}
