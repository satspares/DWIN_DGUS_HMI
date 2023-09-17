#include <Arduino.h>
//include after DWIN hmi(DGUS_SERIAL etc..

// Read word from VP address
u_int16_t readVP(u_int16_t vpAddress)
{
    return (hmi.readVPByte(vpAddress,1) << 8) + hmi.readVPByte(vpAddress) ;
}

// Read long value from VP address
unsigned long readLongValue(u_int16_t vpAddress){
 
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
float readFloatValue(u_int16_t vpAddress){
 
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
void writeLongValue(u_int16_t vpAddress,unsigned long lValue){
    hmi.setVPWord(vpAddress,(lValue >> 16));
    hmi.setVPWord((vpAddress+1), (lValue & 0xFFFF));
}
