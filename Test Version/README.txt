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
