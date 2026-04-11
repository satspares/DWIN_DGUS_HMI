/*
* DWIN DGUS DWIN Library for Arduino
* This Library Supports all Basic Function
* Created by Tejeet ( tejeet@dwin.com.cn ) 
* Updated by Satspares ( satspares@gmail.com )
* Please Checkout Latest Offerings FROM DWIN 
* Here : https://www.dwin-global.com/
*/


#ifndef DWIN_H
#define DWIN_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#ifndef ARDUINO_ARCH_RP2040 
 #ifndef ESP32
    #include <SoftwareSerial.h>
 #endif   
#endif


#define DWIN_DEFAULT_BAUD_RATE      115200
#define ARDUINO_RX_PIN              10
#define ARDUINO_TX_PIN              11

//#define FORCEHWSERIAL


// Thinary Nano 4808 MegaCoreX
#if defined(__AVR_ATmegax08__)
 #define ARDUINO_AVR_NANO_EVERY
#endif


class DWIN{

public:
    // Using AVR Board with Hardware Serial
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(FORCEHWSERIAL)
     DWIN(HardwareSerial& port,long baud=DWIN_DEFAULT_BAUD_RATE);
    
    // Using Pico Board
    #elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_AVR_NANO_EVERY)
    DWIN(HardwareSerial& port, long baud , bool initSerial);

    //Using STM32 Arduino
    #elif ARDUINO_ARCH_STM32
    DWIN(HardwareSerial &port);

    // Using ESP32 Board
    #elif defined(ESP32)
    DWIN(HardwareSerial& port, uint8_t receivePin, uint8_t transmitPin, long baud=DWIN_DEFAULT_BAUD_RATE);
    
    // Using ESP8266 Board
    #elif defined(ESP8266)
    DWIN(uint8_t receivePin, uint8_t transmitPin, long baud=DWIN_DEFAULT_BAUD_RATE);
    DWIN(SoftwareSerial& port, long baud=DWIN_DEFAULT_BAUD_RATE);
    DWIN(Stream& port, long baud=DWIN_DEFAULT_BAUD_RATE);

    // Using Arduino Board
    #else
    DWIN(uint8_t rx=ARDUINO_RX_PIN, uint8_t tx=ARDUINO_TX_PIN, long baud=DWIN_DEFAULT_BAUD_RATE);
    #endif


    // PUBLIC Methods

    // dont look for the ack on no response kernels
    void ackDisabled(bool noACK);
    void echoEnabled(bool enabled);
    // if true return word from rx event not byte
    void returnWord(bool retWord);
    // Listen Touch Events & Messages from HMI
    void listen();
    // Get Version
    double getHWVersion();
    //get GUI software version
    double getGUISoftVersion();
    // restart HMI
    void restartHMI();
    // set Particular Page
    void setPage(byte pageID);
    // get Current Page ID
    byte getPage();
    // set LCD Brightness 0x0-0x64
    void setBrightness(byte pConstrast);
    // get LCD Brightness 
    byte getBrightness();
    // set Data on VP Address
    void setText(long address, String textData);
    // // Set Byte Data on VP Address makes more sense
    void setVPByte(long address, byte data); //alias of below
    void setVP(long address, byte data);
    // read byte from VP Address if bool = true read HiByte
    byte readVPByte(long address, bool = 0);
    // Set WordData on VP Address
    void setVPWord(long address, int data);
    // Read word from VP address returns directly using high and low byte
    uint16_t readVP(long address);
    // read WordData from VP Address you can read sequential multiple words (data returned in rx event) 
    void readVPWord(long address, byte numWords);
    // read or write the NOR from/to VP must be on a even address 2 word are written or read
    void norReadWrite(bool write,long VPAddress,long NORAddress);
     // Play a sound
    void playSound(byte soundID);
    // write long value to VP  eg. change screen brightness
    // #define LCDBackLightLow ((unsigned long)0x641503E8)    
    // #define LCDBackLightAddress 0x0082
    // writeLongValue(LCDBackLightAddress,LCDBackLightLow);
    void writeLongValue(uint16_t vpAddress,unsigned long lValue);
    // beep Buzzer for 1 sec
    void beepHMI(uint16_t beep_time = 1000);
    // Enable/Disable beep Buzzer when touch panel is pressed
    void setTPBeep(bool enable = true);
    // set the hardware RTC The first two digits of the year are automatically added
    void setRTC( byte year, byte month, byte day, byte hour, byte minute, byte second);
    // update the software RTC The first two digits of the year are automatically added
    void setRTCSOFT( byte year, byte month, byte day, byte weekday, byte hour, byte minute, byte second);
    // set text color (16-bit RGB) on controls which allow it ie. text control.
    //  changes the control sp address space (sp=description pointer) content see the DWIN docs.  
    void setTextColor(long spAddress, long spOffset, long color);
    //set float value to 32bit DATA Variable Control  
    void setFloatValue(long vpAddress, float fValue);

    // Send array to the display we dont need the 5A A5 or 
    // the size byte hopefully we can work this out.
    //byte hmiArray[] = {0x83,0x10,0x00,0x1};        // Read 0x1000 one word returns in the rx event
    //byte hmiArray[] = {0x82,0x88,0x00,0x55,0xAA};  // Write 0x8800
    //hmi.sendArray(hmiArray,sizeof(hmiArray));
    void sendArray(byte dwinSendArray[],byte arraySize);

    // Send int array to the display we dont need the 5A A5 or size - words only
    // eg. Using Basic Graphic Control vp 0x5000 draw rectangle
    //  uint16_t intArrayRect[] = {0x5000,0x0003,0x0001,200,100,650,400,0xFFF0,0xFF00};
    //  Fill it with Yellow
    //  uint16_t intArrayFill[] = {0x5000,0x0004,0x0001,200,100,650,400,0xFFF0,0xFF00};
    //  display it
    //  hmi.sendIntArray(0x82,intArrayRect,sizeof(intArrayRect));
    //  hmi.sendIntArray(0x82,intArrayFill,sizeof(intArrayFill));
    void sendIntArray(uint16_t instruction,uint16_t dwinIntArray[],byte arraySize);


    // Callback Function
    typedef void (*hmiListener) (String address, int lastBytes, String message, String response);

    // CallBack Method
    void hmiCallBack(hmiListener callBackFunction);

    // Init the serial port in setup useful for Pico boards
    void initSerial(HardwareSerial& port, long baud);


private:

    #ifndef ARDUINO_ARCH_RP2040 
     #ifndef ESP32
     SoftwareSerial* localSWserial = nullptr; 
     #endif   
     #endif

    Stream* _dwinSerial;   // DWIN Serial interface
    bool _isSoft;          // Is serial interface software
    long _baud;            // DWIN HMI Baud rate
    bool _echo = false;    // Response Command Show
    bool _isConnected;     // Flag set on successful communication
    bool _noACK = false;   // No ack used with no response kernel 
    bool _retWord = false; // return word from rx event when true 

    bool cbfunc_valid;
    hmiListener listenerCallback;

    void init(Stream* port, bool isSoft); 
    byte readCMDLastByte(bool hiByte = 0);
    String readDWIN();
    String handle();
    String checkHex(byte currentNo);
    void flushSerial();

};



#endif // DWIN_H
