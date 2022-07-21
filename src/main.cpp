#include <M5StickCPlus.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "****";
const char* password = "*****";

const char* ntpServer = "ntp.nict.jp";
const long gmtoffset_sec = 0;
const int daylightoffset_sec = 0;

void printLocalTime();

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nCONNECTED!");
  configTime(gmtoffset_sec,daylightoffset_sec,ntpServer);
  printLocalTime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(20);
}

void loop() {
  delay(1000);
  M5.Lcd.setCursor(0,25);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  M5.Lcd.println(&timeinfo, "%A,%B %d \n%Y %H:%M:%S");
}