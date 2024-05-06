#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <DS3231.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <Wire.h>

DS3231 myRTC;

const char *ssid     = "Tuff Gong";
const char *password = "hmmrsprcOUO";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "b.st1.ntp.br", -10800, 60000);

U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 14, /* cs=*/ 12, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);

bool century = false;
bool h12Flag;
bool pmFlag;

char hora[3];
char minuto[3];
char segundo[3];

byte digits_offset_perc; // y offset for the individual digits - percentage 0-100%

float y_offset;

void setup() {
   // Start the serial port
  Serial.begin(57600);

  // Start the I2C interface
  Wire.begin();

// Connect to wifi
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  // Start Ntp Client
  timeClient.begin();

  // Atualiza hora do ntp
  timeClient.update();

  // Atualiza o rtc
  myRTC.setEpoch(timeClient.getEpochTime());
  
  // put your setup code here, to run once:
  u8g2.begin(); // begin function is required for u8g2 library
  u8g2.setContrast(10*16); // set display contrast 0-255
  u8g2.clearBuffer();  // clear the internal u8g2 memory

  delay(500);
}

void loop() {
  //
  int hora_ = myRTC.getHour(h12Flag, pmFlag);
  int minuto_ = myRTC.getMinute();
  int segundo_ = myRTC.getSecond();

  sprintf(hora, "%02d", hora_, DEC);
  sprintf(minuto, "%02d", minuto_, DEC);
  sprintf(segundo, "%02d", segundo_, DEC);
  
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);
  Serial.print(":");
  Serial.println(segundo);

  u8g2.clearBuffer();  // clear the internal u8g2 memory
  u8g2.setFont(u8g2_font_minuteconsole_tr); // choose a suitable font with digits 3px 
  u8g2.setFontDirection(3);
  
  u8g2.drawStr(7, 6, hora);
  u8g2.drawStr(15, 6, minuto);
  u8g2.drawStr(23, 6, segundo);

  u8g2.sendBuffer(); // transfer internal memory to the display

  delay(1000);
}
