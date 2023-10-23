#include <Arduino.h>
#include <DWIN_Arduino.h>

#define PINRX 16
#define PINTX 17
#define DGUS_BAUD 115200
#define DGUS_SERIAL Serial2
#define StartSound 2
#define NORWRITE 1
#define NORREAD 0

DWIN hmi(DGUS_SERIAL, PINRX, PINTX, DGUS_BAUD);
// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response);
unsigned long getWordReply(String response, byte bytesBack);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("DWIN HMI ~ Quick NOR Read Write Example");
  Serial.println("");
  hmi.echoEnabled(false);
  hmi.hmiCallBack(onHMIEvent);
  delay(2000);
  //hmi.setPage(1);
  hmi.playSound(StartSound);

  hmi.norReadWrite(NORREAD,0x4000,0x10);  // read nor addresses 0x10-0x11 into VPs 0x4000-0x4001 (will be random on first run)
  hmi.readVPWord(0x4000,2);               // read VPs 0x4000-0x4001 causes a onHMIEvent callback event  
  hmi.listen();                           // normally in the loop but we are not there yet.

  hmi.setVPWord(0x2000,0xaa55);           // write some stuff to VPs 0x2000-0x2001
  hmi.setVPWord(0x2001,0xbb66);
  // norReadWrite must be an even addresses see the DWIN docs.
  hmi.norReadWrite(NORWRITE,0x2000,0x10); // write the 2 words in VPs 0x2000-0x2001 into NOR address 0x10-0x11
  hmi.norReadWrite(NORREAD,0x3000,0x10);  // read the 2 words in nor address 0x10-0x10 into VPs 0x3000-0x3001
  hmi.readVPWord(0x3000,2);               // read VPs 0x3000-0x3001 causes a onHMIEvent callback event
}

// our loop
void loop() {
   hmi.listen();   
}

/**
 * 
 * DWIN Display callback event
 * 
 */
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");

  if ((address.toInt() == 3000) || (address.toInt() == 4000)){
    Serial.println("");
    Serial.println("Address 0x" + address);
    Serial.println(getWordReply(response,0),HEX);
    Serial.println(getWordReply(response,2),HEX);
  }

}  

/**
 * 
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
//  for(int i = 0; i < index; i++)
//  {
//   /Serial.println(strings[i]);
//  }
  index --;
  index = index - bytesBack;
  return((unsigned long) strtol(strings[index-1], NULL, 16) << 8) + (unsigned long) strtol(strings[index], NULL, 16);
}




