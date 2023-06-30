# DWIN DGUS HMI Arduino Library
Arduino Library for DWIN DGUS T5L HMI Display
Supporting Features till date.
- getHWVersion
- restartHMI()
- setPage()
- getPage()
- setBrightness()
- getBrightness()
- setVP()
- setText()
- beepHMI()
- listenEvents()
- playSound() 

30-June-2023 added playSound() changed lib. name to DWIN_Arduino to avoid naming clashes
added hardware serial for Arduino Mega etc.
## Usage
Download the Library and extract the folder in the libraries of Arduino IDE
#### Include DWIN Library (eg. DWIN_Arduino.h) 
```C++
#include <DWIN_Arduino.h>
```

#### Initialize the hmi Object with Rx | Tx Pins and Baud rate
```C++
// If Using ESP32 Or Arduino Mega 
#if defined(ESP32)
  #define DGUS_SERIAL Serial2
  DWIN hmi(DGUS_SERIAL, 16, 17, DGUS_BAUD); // 16 Rx Pin | 17 Tx Pin
// If Using Arduino Uno
#else
  DWIN hmi(2, 3, DGUS_BAUD);    // 2 Rx Pin | 3 Tx Pin
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

---
