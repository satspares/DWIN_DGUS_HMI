
/*
 * -----------------------------------------------------------------------------------------
 * DWIN Sketch  
 * -----------------------------------------------------------------------------------------
 * This is DWIN HMI Library for Arduino Compatible Boards. More Info about Display Visit
 * Official Site --> https://www.dwin-global.com/
 *
 * Example sketch/program showing how to initialize DWIN Hmi with Arduino - ESP32 - Pico Boards
 * In this example we can see on setup loop we change the page no and set the brightness &
 * Listen to display Events from serial port
 *
 * DWIN HMI to Various Boards Pinout Connection
 * -----------------------------------------------------------------------------------------
 * DWIN           ESP32       Arduino    ArduinoMega   Mega Hardware Serial Pico
 * Pin             Pin           Pin           Pin              Pin
 *             ESP32 and Pico 3.3v-5v Logic Shift needed TXS0108E etc.  
 * -----------------------------------------------------------------------------------------
 * 5V              Vin           5V            5V               5V          5V
 * GND             GND           GND           GND              GND         GND
 * RX2             16            2             18               17          9
 * TX2             17            3             19               16          8
 *------------------------------------------------------------------------------------------
 *
 * For More information Please Visit : https://www.dwin-global.com/
 *
 */

#include <Arduino.h>
#include <DWIN_Arduino.h>

#define DGUS_BAUD 115200

//#define FORCEHWSERIAL
//using Boards with Hardware serial you may need extra
// defines in here and DWIN_Arduino.h eg. Teensyduino, nano (only 1 serial port) etc
// or #define FORCEHWSERIAL here and in DWIN_Arduino.h
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(FORCEHWSERIAL)
  #define DGUS_SERIAL Serial2
  DWIN hmi(DGUS_SERIAL, DGUS_BAUD);

//platform = https://github.com/platformio/platform-raspberrypi.git Arduino Mbed
#elif defined(ARDUINO_ARCH_RP2040)  && defined(__MBED__)
#define TXPIN 8
#define RXPIN 9
#define DGUS_SERIAL Serial2
UART Serial2(TXPIN, RXPIN, 0 , 0);
DWIN hmi(Serial2, DGUS_BAUD, false);     // the false do not init the port yet


//board_build.core = earlephilhower 
//platform = https://github.com/maxgerhardt/platform-raspberrypi.git
#elif defined(ARDUINO_ARCH_RP2040)  && !defined(__MBED__)
#define TXPIN 8
#define RXPIN 9
#define DGUS_SERIAL Serial2
DWIN hmi(DGUS_SERIAL, DGUS_BAUD, false); // the false do not init the port yet

// If Using ESP 32
#elif defined(ESP32)
#define DGUS_SERIAL Serial2
DWIN hmi(DGUS_SERIAL, 16, 17, DGUS_BAUD);


// If Using Arduino Uno
#else
DWIN hmi(2, 3, DGUS_BAUD);
#endif
// include the helpers
#include "DWIN_Arduino_Helpers.hpp"
// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response);
unsigned long getWordReply(String response, byte bytesBack);
uint16_t readVP(uint16_t vpAddress);
//set some variable values
uint16_t v1000var = 0x55AA;
uint16_t v1001var = 0x6677;

void setup()
{
   Serial.begin(9600);
   delay(3000);
   // arduino raspberry pi pico does not like ports touched before setup
   #if defined(ARDUINO_ARCH_RP2040)  && !defined(__MBED__)
   Serial2.setRX(RXPIN);
   Serial2.setTX(TXPIN);   
   hmi.initSerial(Serial2, DGUS_BAUD );
   #endif
   #if defined(ARDUINO_ARCH_RP2040)  && defined(__MBED__)
   hmi.initSerial(Serial2, DGUS_BAUD );
   #endif
  
  Serial.println("DWIN HMI ~ Read Vars from vp's");
  Serial.print("DWIN Hardware Version ");
  Serial.println(hmi.getHWVersion());
  hmi.echoEnabled(false);
  hmi.hmiCallBack(onHMIEvent);
  hmi.setPage(1);
  hmi.setBrightness(0x20); // see the docs 0x82 LED_Config
}

void loop()
{
  // Listen HMI Events
  hmi.listen();
  // note write before read as we have a delay at end of loop
  // write some values in vp 0x1000 and vp 0x1001
  hmi.setVPWord(0x1000,v1000var);
  hmi.setVPWord(0x1001,v1001var); 
  //read one word from vp 0x1000
  hmi.readVPWord(0x1000, 1);
  //or read 2 words back from vp's 0x1000-0x1001
  //hmi.readVPWord(0x1000, 2);
  
  // or the easy way without the event
  Serial.println(readVP(0x1000));

  //increment our vars before write at top of loop 
  v1000var ++;
  v1001var ++;
  delay(1000);
} // End our loop


// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");
  if (address == "1000")
  { 
     Serial.println("");
     Serial.print("0x");
     Serial.println(getWordReply(response,0),HEX);
    //or print the word at 0x1001 if hmi.readVPWord(0x1000, 2); in the loop
    //Serial.print("0x");
    //Serial.println(getWordReply(response,2),HEX);
  }
}


