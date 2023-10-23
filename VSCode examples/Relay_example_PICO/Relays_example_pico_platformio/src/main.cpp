  
#include <Arduino.h>
#include <DWIN_Arduino.h>
#include <Ticker.h> //https://github.com/sstaub/Ticker

#define DGUS_BAUD 115200
#define TXPIN 8
#define RXPIN 9
#define DGUS_SERIAL Serial2
DWIN hmi(DGUS_SERIAL, DGUS_BAUD, false); // false do not init the port yet

int relayPins[8] = {10, 11, 12, 13, 14, 15, 16, 17};  // just a guess
byte counter;
byte lastByteCopy;

// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response);
void iconUpdate();
Ticker timerIconTicker(iconUpdate,1500);

void setup()
{
  Serial.begin(9600);
  Serial2.setRX(RXPIN);
  Serial2.setTX(TXPIN);
  hmi.initSerial(Serial2, DGUS_BAUD );
  Serial.println("DWIN HMI rasperry pi pico ~ Relay Example");
  hmi.restartHMI();                        // restart the display, not normally needed but useful if debugging
  delay(3000);
  counter = 0;
  while(counter < 8)                       // Number of relays 8, make all outputs LOW
  {
    pinMode(relayPins[counter], OUTPUT);
    digitalWrite(relayPins[counter],LOW) ; 
    counter ++;
  }
  hmi.echoEnabled(false);
  hmi.hmiCallBack(onHMIEvent);
  hmi.setVP(0x1000,0xFF);                  // Put all icons to the off state on VP 0x1000
  hmi.setPage(7);                          // The page with switches
  Serial.println("End of Setup");
}

  void loop()
  {                         
  hmi.listen();                            // Listen HMI Events

  if (timerIconTicker.state() == RUNNING){ // if Ticker is running update it
   timerIconTicker.update();
  }
  
  } 


// Callback event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");
  if (address == "1000" && (lastByte <= 0xFF))          // our switches control VP address this is a bit control
  {                                                     // If lastByte > 0xFF the serial data is incorrect
    delay(100);                                         // higher delay may be needed here or the relays may bounce
    counter = 0;
    while(counter < 8)                                  // Number of relays
    {
      ((lastByte >> counter) & 1) ? digitalWrite(relayPins[counter],LOW) : digitalWrite(relayPins[counter],HIGH) ; 
      (digitalRead(relayPins[counter])) ? Serial.println ("Relay " + String(counter+1) + " ON") : Serial.println ("Relay " + String(counter+1) + " OFF") ;
      counter ++;
    }
    lastByteCopy = lastByte;
    timerIconTicker.start();
    timerIconTicker.interval(1500);
   } 
  }

// Ticker update the icons 1.5 seconds after last transmission from the display
// Try to keep the icons insync we can type faster than the serial?
// We should poll the display and compare it with the relays 
// and turn off the auto update on the icons 
void iconUpdate(){
static byte oldIconStatus;
if (lastByteCopy != oldIconStatus ){
  oldIconStatus = lastByteCopy;
  hmi.setVP(0x1000,lastByteCopy);  
  Serial.println("icon update");
}
 timerIconTicker.stop();
}  
