/**
 * @file TIME.cpp
 * @brief Test Zeit über WLAN ohne NTP auf ESP32
 * @version 1.0
 * @date 1 Dez 2024
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2024 B. Borys
 */
// #define _USE_TIME_
#ifdef _USE_TIME_
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
//  Zeitserver aus dem Pool: #define Zeitserver "de.pool.ntp.org"
//  Zeitserver lokal:
#define Zeitserver "192.168.1.1"
#define Zeitzone "CET-1CEST,M3.5.0/02,M10.5.0/03"
void setup()
{
  int i = 0;
  Serial.begin(115200);
  // Parameter für die Zeit
  configTzTime(Zeitzone, Zeitserver);
  WiFi.begin(WLANSSID, WLANPWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    log_w("WLAN?");
    delay(2000);
    if (i++ > 30)
    {
      log_e("WLAN Error %d", WiFi.status());
      while (true)
        ;
    }
  }
  log_i("WLAN OK");
  WiFi.printDiag(Serial);
}
static unsigned long Vorher = 0;
void loop()
{
  time_t ZeitTime;
  time(&ZeitTime); // nach etwa 30 Sek stimmt die Zeit, sofern WiFi noch läuft
  log_i("aktuelle Zeit %d", ZeitTime);
  if (ZeitTime < 4000)
    log_w("ungültig, wiederholen");
  else
  {
    tm ZeitLocal;
    WiFi.disconnect(true);
    // localtime_r -> Zeit in die lokale Zeitzone setzen
    localtime_r(&ZeitTime, &ZeitLocal);
    Serial.print("Zeit ");
    Serial.print(ZeitLocal.tm_mday);
    Serial.print(".");
    Serial.print(ZeitLocal.tm_mon + 1);
    Serial.print(".");
    // tm_year = Anzahl Jahre seit 1900
    Serial.print(ZeitLocal.tm_year + 1900);
    Serial.print(" ");
    Serial.print(ZeitLocal.tm_hour);
    Serial.print(":");
    if (ZeitLocal.tm_min < 10)
      Serial.print("0");
    Serial.print(ZeitLocal.tm_min);
    Serial.print(":");
    if (ZeitLocal.tm_sec < 10)
      Serial.print("0");
    Serial.print(ZeitLocal.tm_sec);
    Serial.println();
    Serial.print("Tage seit 1. Januar: ");
    Serial.println(ZeitLocal.tm_yday);
  }
  delay(10000);
}
#endif