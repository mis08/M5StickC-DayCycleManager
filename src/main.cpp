#include <M5StickCPlus.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "***";
const char* password = "***";

const char* ntpServer = "ntp.nict.jp";
const long gmtoffset_sec = 32400;
const int daylightoffset_sec = 0;

int OFFSET_H = 0;
int OFFSET_M = 0;

void printLocalTime();
void printMyCycleTime(struct tm &timeinfo);

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.lcd.setTextSize(2);
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
  M5.Lcd.setCursor(0,64);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  M5.Lcd.println(&timeinfo, "%A,%B %d \n%Y %H:%M:%S");
  printMyCycleTime(timeinfo);
}

void printMyCycleTime(struct tm &timeinfo){
  M5.update();
  if(M5.BtnA.wasReleased()){
    OFFSET_H = 6 - timeinfo.tm_hour;
    OFFSET_M = 0 - timeinfo.tm_min;
  }
  timeinfo.tm_hour += OFFSET_H;
  timeinfo.tm_min += OFFSET_M;
  M5.Lcd.println(&timeinfo, "\nCycleTIme : %H:%M:%S");
}