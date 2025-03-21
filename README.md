DWIN DGUS HMI Arduino Library forked from dwinhmi/DWIN_DGUS_HMI
Arduino Library for DWIN DGUS T5L HMI Display
Supporting Features.
- getHWVersion()
- getGUISoftVersion()
- restartHMI()
- setPage()
- getPage()
- setBrightness()
- getBrightness()
- setVPByte()
- setText()
- beepHMI()
- setTPBeep()
- listenEvents()
- playSound() 
- setRTC()
- setRTCSOFT()
- norReadWrite()
- setVPWord()
- readVP()
- readVPWord()
- readVPByte()   //see .h file
- setTextColor()
- setFloatValue()
- sendArray()   // see .h file

30-June-2023 added playSound() changed lib. name to DWIN_Arduino to avoid naming clashes
added hardware serial for Arduino Mega etc.

01-July-2023 added setRTC and setRTCSOFT

06-July-2023 added ESP32 set Software RTC Demo

08-July-2023 added ESP32 NOR Read Write Example

10-July-2023 added using relays on Arduino UNO Example 

14-July-2023 add rasperry pi pico example

22-July-2023 add readVPByte Function

04-September-2023 added setTextColor Function

07-September-2023 added setFloatValue Function

15-September-2023 add Test Version Directory

23-September-2023 add STM32 to Library

23-September-2023 add some helper functions in DWIN_Arduino_Helpers.txt

23-September-2023 add sendArray Function

07-April-2024 Added nano every and hello world example

15-February-2025 add include for Thinary Nano 4808 MegaCoreX

21-March-2025 fix beep functions move readVP() into main library from DWIN_Arduino_Helpers.hpp

## Usage
Download the Library and extract the folder in the libraries of Arduino IDE,
 see example for rasperry pi pico
#### Include DWIN Library (eg. DWIN_Arduino.h) 
```C++
#include <DWIN_Arduino.h>
#define DGUS_BAUD 115200

//#define FORCEHWSERIAL
//using Boards with Hardware serial you may need extra
// defines in here and DWIN_Arduino.h eg. Teensyduino, nano (only 1 serial port) etc
// or #define FORCEHWSERIAL here and in DWIN_Arduino.h
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(FORCEHWSERIAL)
  #define DGUS_SERIAL Serial2
  DWIN hmi(DGUS_SERIAL, DGUS_BAUD);

// If Using ESP 32
#elif defined(ESP32)
#define DGUS_SERIAL Serial2
DWIN hmi(DGUS_SERIAL, 16, 17, DGUS_BAUD);

// If Using Arduino Uno
#else
DWIN hmi(2, 3, DGUS_BAUD);
#endif

```

#### Define callback Function
```C++
// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response){  
  Serial.println("OnEvent : [ A : " + address + " | D : "+ String(lastByte, HEX)+ " | M : "+message+" | R : "+response+ " ]"); 
  if (address == "1002"){
  // Take your custom action call
  }
}
```

#### In void setup()
```C++
  hmi.echoEnabled(false);      // To get Response command from HMI
  hmi.hmiCallBack(onHMIEvent); // set callback Function
```

#### In loop()
```C++
  // Listen HMI Events
  hmi.listen();
```

### Brightness Control
```C++
  // Turn off Backlight
  hmi.setBrightness(0);

  // Turn on Backlight
  hmi.setBrightness(100);

  // Get Backlight level
  int brightness = hmi.getBrightness();
```
---
