#include <TFT_eSPI.h> // screen library
#include <SPI.h> // SPI bus library
#include <NTPClient.h> // NTP library
#include <WiFiUdp.h>
#include "WiFi.h"
#include <Wire.h> // I2C bus library

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4  // Display backlight control pin
#define ADC_EN          14
#define ADC_PIN         34
#define BUTTON_1        35 // not used
#define BUTTON_2        0  // not used

// ***********************************
// ********* WIRING ******************
// ***********************************
// RTC DS3231 connection
// VCC on 3V
// I2C wire SDA is pin 21
// I2C wire SCL is pin 22

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

// **************************************************************
// ******* Fill the network credentials with your owns **********
//***************************************************************

const char* ssid     = "xxxxxx";
const char* password = "yyyyyyy";

// NTP
WiFiUDP ntpUDP;
// ************************************************************
// i'm in PDT which is +2 from GMT in summer time. Put your own
NTPClient timeClient(ntpUDP, "us.pool.ntp.org", +2*3600);

// RTC Libraries
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

// ************************************
// ** RTC Setup
// ************************************
void RTC_Update(){
  // Do udp NTP lookup, epoch time is unix time - subtract the 30 extra yrs (946684800UL) library expects 2000
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime()-946684800UL;
  Rtc.SetDateTime(epochTime);
}
bool RTC_Valid(){
  bool boolCheck = true;
  if (!Rtc.IsDateTimeValid()){
    Serial.println("RTC lost confidence in the DateTime!  Updating DateTime");
    boolCheck = false;
    RTC_Update();
  }
  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now.  Updating Date Time");
    Rtc.SetIsRunning(true);
    boolCheck = false;
    RTC_Update();
  }
}


char buff[512];
int Trials;
int First;


void setup()
{
    Serial.begin(115200);
    Serial.println("Start");
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);

    if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
         pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
         digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }

 // Try to connect to Wi-Fi network with SSID and password (10 times max)
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Trials = 0;
  while ((WiFi.status() != WL_CONNECTED) && (Trials < 10)) {
    delay(500);
    Trials = Trials + 1;
    Serial.print(".");
  }
  // Print on on-borad screen local IP address if the connection to WiFi was successfull.
  if (WiFi.status() == WL_CONNECTED) {
  tft.setRotation(1); // rotation F6CZV
  tft.setCursor(0, 0); // F6CZV
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  String SSIDC = "WiFi connected to \n" + String(ssid);
  tft.println(SSIDC); // F6CZV
  tft.println("IP address: "); // F6CZV
  tft.println(WiFi.localIP()); // F6CZV
  delay(1000);
  }

  // Start NTP Time Client only if the connection to WiFi was successfull.
 Rtc.Begin();
 if (WiFi.status() == WL_CONNECTED) {
   timeClient.begin();
   delay(2000);
   timeClient.update();
   RTC_Update();
   }

   tft.fillScreen(TFT_BLACK);
   tft.setTextColor(TFT_WHITE);
   First = 1;
}

void loop()
{ if (!RTC_Valid()){
    RTC_Update();
  }
  RtcDateTime currTime = Rtc.GetDateTime();
  printDateTime(currTime);
 // Serial.println(timeClient.getFormattedTime());
  delay(1000);
  First = 2;
}
// Utility print function
#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
    char datestringjour[11];
    char datestringheure[9];
    char datestringsecond[3];

            snprintf_P(datestringjour,
            countof(datestringjour),
            PSTR("%02u/%02u/%04u"),
            dt.Day(),
            dt.Month(),
            dt.Year() );

    snprintf_P(datestringheure,
            countof(datestringheure),
            PSTR("%02u:%02u:%02u"),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );

    snprintf_P(datestringsecond,
            countof(datestringsecond),
            PSTR("%02u"),
            dt.Second() );

   // date is printed if it is the first display or a new day

    if (((dt.Hour() == 0) && (dt.Minute() == 0) && (dt.Second() == 0))|| (First == 1)){
        tft.setTextSize(2);
        tft.setCursor(0, 90);
        tft.fillRect(0, 90, 200,50, TFT_BLACK); // x, y, width, height, color
        tft.println(datestringjour);
        Serial.println("prem");
        }

    // if the second counter is different from 0 only the second display zone is cleared
    // to avoid screen flicker

     if (dt.Second()!= 0 ) {
        tft.fillRect(180, 30, 60,60, TFT_BLACK);
        tft.setTextSize(5);
        tft.setCursor(180, 30);
        tft.println(datestringsecond);
        }

    // The hour, minute, second are printed every minute or the first time

    if ((dt.Second()== 0 )|| (First == 1)) {
        tft.fillRect(0, 30, 240,60, TFT_BLACK); //240
        tft.setTextSize(5);
        tft.setCursor(0, 30);
        tft.println(datestringheure);
        }
}
