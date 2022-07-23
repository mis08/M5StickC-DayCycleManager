#include <M5StickCPlus.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "***";
const char* password = "***";

const char* ntpServer = "ntp.nict.jp";
const long gmtoffset_sec = 32400;
const int daylightoffset_sec = 0;

long OFFSET = 0;

void printLocalTime();
void printMyCycleTime(struct tm &timeinfo);

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(7);
  M5.Lcd.setRotation(3);
  M5.Lcd.printf("Connecting to %s", ssid);
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
  setCpuFrequencyMhz(20);
  delay(20);
}

void loop() {
  delay(1000);
  printLocalTime();
}

void printLocalTime(){
  M5.Lcd.setCursor(0,24);
  M5.Lcd.setTextSize(2);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  M5.Lcd.println(&timeinfo, "\nN: %H:%M:%S");
  printMyCycleTime(timeinfo);
}

void printMyCycleTime(struct tm &timeinfo){
  M5.update();
  long current =  (timeinfo.tm_hour * 60) + timeinfo.tm_min;
  if(M5.BtnA.wasReleased()){
    int of_H = 6 - timeinfo.tm_hour;
    int of_M = 0 - timeinfo.tm_min;
    OFFSET = (of_H * 60) + of_M;
  }
  current += OFFSET;
  char time[17] = {0};
  M5.Lcd.setTextSize(5);
  sprintf(time, "%02u:%02u:%02u", current / 60, abs(current % 60), timeinfo.tm_sec );

  M5.Lcd.println(time);
}