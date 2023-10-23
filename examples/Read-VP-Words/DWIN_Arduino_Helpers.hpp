//copy required function(s) into your code
//or include after
//DWIN hmi(DGUS_SERIAL, 115200 ... etc
//#include "DWIN_Arduino_Helpers.hpp"

// Read word from VP address
uint16_t readVP(uint16_t vpAddress)
{
    return (hmi.readVPByte(vpAddress,1) << 8) + hmi.readVPByte(vpAddress) ;
}

// Read long value from VP address
unsigned long readLongValue(uint16_t vpAddress){
 
    byte data[4] = {0,0,0,0};
    unsigned long lValue;

    data[2] = hmi.readVPByte(vpAddress);
    data[3] = hmi.readVPByte(vpAddress,1);
    data[0] = hmi.readVPByte((vpAddress+1));
    data[1] = hmi.readVPByte((vpAddress+1),1);
    memcpy(&lValue,data,4);
    return lValue;
}

// Read float value from VP address
float readFloatValue(uint16_t vpAddress){
 
    byte data[4] = {0,0,0,0};
    float fValue;

    data[2] = hmi.readVPByte(vpAddress);
    data[3] = hmi.readVPByte(vpAddress,1);
    data[0] = hmi.readVPByte((vpAddress+1));
    data[1] = hmi.readVPByte((vpAddress+1),1);
    memcpy(&fValue,data,4);
    return fValue;
}

// Write long value to VP address
void writeLongValue(uint16_t vpAddress,unsigned long lValue){
    hmi.setVPWord(vpAddress,(lValue >> 16));
    hmi.setVPWord((vpAddress+1), (lValue & 0xFFFF));
}


/**
 * Used in the callback event onHMIEvent
 * Get the last 2 bytes returned from hmi for our use if bytesBack = 0
 * If bytesBack = 2 get the 2 previous bytes etc. 
 */
unsigned long getWordReply(String response, byte bytesBack){
  int str_len;
  char *ptr = NULL;
  str_len = response.length() + 1; 
  char *strings[str_len];  // we dont need this much memory
  char response_array[str_len];
  response.toCharArray(response_array,str_len);
  ptr = strtok(response_array, " " );

  byte index = 0;
  while(ptr != NULL)
  {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, " ");
  }
  index --;
  index = index - bytesBack;
  return((unsigned long) strtol(strings[index-1], NULL, 16) << 8) + (unsigned long) strtol(strings[index], NULL, 16);
}


// noWords should be your char length / 2
// eg. read 12 chars Serial.println(readVPText(0x2300,6)) ;
String readVPText(uint16_t vpAddress,byte noWords){
  int i = 0;
  bool nextByte = true; //true = read first char
  byte byteRead = 0;
  String textMessage;
  while(i < noWords){
    byteRead = hmi.readVPByte((vpAddress+i),nextByte);
    // 128 can be 254 for extended ascii
    if ((byteRead < 32) || (byteRead > 128)){
      break;
    }
    textMessage += char(byteRead);
    nextByte = ! nextByte;
    if (nextByte){i++;}
  }
  return textMessage;
}
