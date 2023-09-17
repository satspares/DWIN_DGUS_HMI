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


Added some helpers in DWIN_Arduino_Helpers.h
// Read word from VP address
u_int16_t readVP(u_int16_t vpAddress)

// Read long value from VP address
unsigned long readLongValue(u_int16_t vpAddress)

// Read float value from VP address
float readFloatValue(u_int16_t vpAddress)

// Write long value to VP address
void writeLongValue(u_int16_t vpAddress,unsigned long lValue)
