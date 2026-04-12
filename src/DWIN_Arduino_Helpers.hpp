//copy required function(s) into your code
//or include after
//DWIN hmi(DGUS_SERIAL, 115200 ... etc
//#include "DWIN_Arduino_Helpers.hpp"


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
    hmi.setVPWord((vpAddress+1), (lValue & 0xFFFF));
    hmi.setVPWord(vpAddress,(lValue >> 16));
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


const byte rotate0=0; const byte rotate90=1;
const byte rotate180=2; const byte rotate270=3;
//rotate the screen  eg. setScreenRotate(rotate270); // flip the screen
void setScreenRotate(byte angle)
{
   hmi.setVPWord(0x0081,((hmi.readVP(0x0081) & 0xFFFC) + angle));
   hmi.setVPWord(0x0080,0x5A00);  // set it.
}
