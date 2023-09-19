// hmi.readVPByte can now read hi or low byte
  hmi.setVPWord(0x2000,0xAA55);
  Serial.println(hmi.readVPByte(0x2000),HEX);   //optional bool parameter or hmi.readVPByte(0x2000,0)
  Serial.println(hmi.readVPByte(0x2000,1),HEX); //read hi byte
  unsigned long vpContent = (hmi.readVPByte(0x2000,1) << 8) + hmi.readVPByte(0x2000) ;
  Serial.println(vpContent,HEX);
  Serial.print("Display OS Software Version ");
  Serial.println(hmi.getHWVersion());
  Serial.print("Display GUI Software Version ");
  Serial.println(hmi.getGUISoftVersion());


// Send array to dwin we dont need the 5A A5 or the size byte
// hopefully we can worh this out.
//byte hmiArray[] = {0x83,0x10,0x00,0x1};       // Read 0x1000 one word returns in the rx event
//byte hmiArray[] = {0x82,0x88,0x00,0x55,0xAA};  // Write 0x1000
hmi.sendArray(hmiArray,sizeof(hmiArray));

Added some helpers in DWIN_Arduino_Helpers.h
// Read word from VP address
uint16_t readVP(uint16_t vpAddress)

// Read long value from VP address
unsigned long readLongValue(u_int16_t vpAddress)

// Read float value from VP address
float readFloatValue(uint16_t vpAddress)

// Write long value to VP address
void writeLongValue(uint16_t vpAddress,unsigned long lValue)
