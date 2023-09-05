#include <Arduino.h>
#include <DWIN_Arduino.h>
#include <WiFi.h>
#include <Timezone.h>
#include <NTPClient.h>
#include <myTicker.h> // clone Ticker.h as it picks up the wrong Ticker.h (esp8266?)

const char* ssid = "SSID";
const char* password = "PASSWORD";
bool clockTimeout = true;
#define KEYPAD 1070
#define REBOOTME 9999
#define PINRX 16
#define PINTX 17
#define DGUS_BAUD 115200
#define DGUS_SERIAL Serial2
#define RSSIVar 0x1010  //txt 21 dec long
#define IPaddressVar 0x1030  //txt 21 dec long
#define StartSound 2

#define NTP_OFFSET   0 
#define NTP_INTERVAL 60 * 1000 * 2   // In miliseconds x 2 is 2 seconds
#define NTP_ADDRESS  "0.uk.pool.ntp.org"  

WiFiUDP ntpUDP;
Ticker clockTicker;

DWIN hmi(DGUS_SERIAL, PINRX, PINTX, DGUS_BAUD);

// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response);
void setRTC_Software(void);
void setup_wifi(void);  
void clockCounter(void); 
int getWordReply(String response);
static tm getDateTimeByParams(long time);
static String getDateTimeStringByParams(tm *newtime, char *pattern = (char *)"%d/%m/%Y %H:%M");
static String getEpochStringByParams(long time,char *pattern = (char *)"%d-%m-%Y-%H-%M");

int GTMOffset = 0; // SET TO UTC TIME
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// US Eastern Time Zone (New York, Detroit)
//TimeChangeRule etDST = {"EDT", Second, Sun, Mar, 2, -240};  // Daylight time = UTC - 4 hours
//TimeChangeRule etSTD = {"EST", First, Sun, Nov, 2, -300};   // Standard time = UTC - 5 hours
//Timezone ourT(etDST, etSTD);

// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};  // Standard Time
Timezone ourT(BST, GMT);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("DWIN HMI ~ Set RTC by Software Demo.");
  hmi.setPage(0);
  setup_wifi();
  clockTicker.attach(60, clockCounter);
  hmi.echoEnabled(false); 
  hmi.hmiCallBack(onHMIEvent);
  delay(2000);
  hmi.playSound(StartSound);
  hmi.setPage(1);
  setRTC_Software();
}
// Our main loop
void loop() {
 hmi.listen(); 
 if (clockTimeout) 
 {
  setRTC_Software() ;
  clockTimeout = false;
 }

}

// HMI Callback
// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");
  if (address == "1070") //KEYPAD
  {
     switch (getWordReply(response)) {
     case REBOOTME:
      delay(500);
      hmi.restartHMI();
      delay(500);
      esp_restart();
     case 1234:
     Serial.println("1234");
      break;
     default:
      break;
  }
 }
}
/**
 * 
 * Setup the wifi
 * 
 */
void setup_wifi()
{
// We start by connecting to a WiFi network
  Serial.println();
  Serial.println("Connecting..");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  byte wifi_count = 0;
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    wifi_count++;
    if (wifi_count > 30) // we cant loop forever
    Serial.println("WiFi connect failed");
    break;
  }
  delay(1000); // Wait for the wifi to be ready
  long rssi = WiFi.RSSI();
  Serial.println("rssi " + String(rssi));
  Serial.println("IP address");
  Serial.println(WiFi.localIP().toString().c_str());
  Serial.println(WiFi.BSSIDstr());

  hmi.setText(RSSIVar,String(rssi));
  hmi.setText(IPaddressVar,WiFi.localIP().toString().c_str());
  delay(2000);

}  

/**
 * 
 *  get the current time and
 *  send to the display
 */
void setRTC_Software()
{
    String Hours, Year, Day, Month;
    byte Minutes, Seconds, WeekDay;
    int fullYear;
    clockTimeout = false;
    if (timeClient.update())
    {
      unsigned long epoch = timeClient.getEpochTime();
      setTime(epoch);
    } 
    Hours = getEpochStringByParams(ourT.toLocal(now()), (char *)"%H");
    Year = getEpochStringByParams(ourT.toLocal(now()), (char *)"%Y");
    Day = getEpochStringByParams(ourT.toLocal(now()), (char *)"%d");
    Month = getEpochStringByParams(ourT.toLocal(now()), (char *)"%m");
    fullYear = Year.toInt();
    Year.remove(0, 2);  // Remove trhe 20 part of the year
    Minutes = timeClient.getMinutes();
    Seconds = timeClient.getSeconds();
    WeekDay = weekday() - 1; // TimeLib weekday returns(1-7)? but we want(0-6)

    if (Seconds){
      clockTicker.attach(61 - Seconds, clockCounter); // Bring minute time more correct
    }

    if (fullYear > 2022 && fullYear < 2070)
    { // make sure its not 1970 or 2070 it does happen!  
    hmi.setRTCSOFT( Year.toInt(), Month.toInt(), Day.toInt(), WeekDay, Hours.toInt(), Minutes, Seconds );

    }
}

/**
 *
 * Clock Ticker timeout reset
 * 
 */

void  clockCounter()
{
 clockTimeout = true; 
}

/**
 * Input tm time format and return String with format pattern
 * by Renzo Mischianti <www.mischianti.org>
 */
static String getDateTimeStringByParams(tm *newtime, char *pattern)
{
  char buffer[30];
  strftime(buffer, 30, pattern, newtime);
  return buffer;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
/**
 * Input time in epoch format and return tm time format
 * by Renzo Mischianti <www.mischianti.org>
 */
static tm getDateTimeByParams(long time)
{
  struct tm *newtime;
  const time_t tim = time;
  newtime = localtime(&tim);
  return *newtime;
}

/**
 * Input time in epoch format form
 * at and return String with format pattern
 * by Renzo Mischianti <www.mischianti.org>
 */
static String getEpochStringByParams(long time, char *pattern)
{
  tm newtime;
  newtime = getDateTimeByParams(time);
  return getDateTimeStringByParams(&newtime, pattern);
}

/**
 * 
 * Get the last 2 bytes returned from hmi for our use
 * 
 */
int getWordReply(String response){
  int str_len;
  char *strings[20];
  char *ptr = NULL;
  str_len = response.length() + 1; 
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
  //for(int i = 0; i < index; i++)
  //{
   //Serial.println(strings[i]);
  //}
  return((int) strtol(strings[index-2], NULL, 16) << 8) + (int) strtol(strings[index-1], NULL, 16);
}
