#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <NTP.h>
#include "espuhrzeit.h"
// NTP-Server aus dem Pool
//#define Zeitserver "de.pool.ntp.org"
#define Zeitserver "192.168.1.1"
#define Zeitzone "CET-1CEST,M3.5.0/02,M10.5.0/03"
// time_t enthält die Anzahl der Sekunden seit dem 1.1.1970 0 Uhr
time_t aktuelleZeit;
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);
/*
  Struktur tm
  tm_hour -> Stunde: 0 bis 23
  tm_min -> Minuten: 0 bis 59
  tm_sec -> Sekunden 0 bis 59
  tm_mday -> Tag 1 bis 31
  tm_wday -> Wochentag (0 = Sonntag, 6 = Samstag)
  tm_mon -> Monat: 0 (Januar) bis 11 (Dezember)
  tm_year -> Jahre seit 1900
  tm_yday -> vergangene Tage seit 1. Januar des Jahres
  tm_isdst -> Wert > 0 = Sommerzeit (dst = daylight saving time)
*/
void setup()
{
  Serial.begin(115200);

  // Parameter für die zu ermittelnde Zeit
  configTzTime(Zeitzone, Zeitserver);
  WiFi.enableIpV6();
  WiFi.begin(WLANSSID, WLANPWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Verbunden mit ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);
ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);
ntp.updateInterval(600000L);// 5 Min
ntp.begin("fritz.box");
ntp.update();
}
void loop()
{
  Startzeit = millis();
  if (Startzeit - GesicherteStartZeit > Intervall)
  {
    Intervall = 30000;
    ZeitAnzeigen();
    GesicherteStartZeit = Startzeit;
    Serial.println(ntp.formattedTime("%d. %B %Y")); // dd. Mmm yyyy
    Serial.println(ntp.formattedTime("%A %T"));     // Www hh:mm:ss
      }
}
void ZeitAnzeigen()
{  
  // aktuelle Zeit lesen
  time(&aktuelleZeit);
  // localtime_r -> Zeit in die lokale Zeitzone setzen
  localtime_r(&aktuelleZeit, &Zeit);
  // Serielle Ausgabe mit Namen des Wochentages
  switch (Zeit.tm_wday)
  {
  case 0:
    Serial.print("Sonntag ");
    break;

  case 1:
    Serial.print("Montag ");
    break;

  case 2:
    Serial.print("Dienstag ");
    break;

  case 3:
    Serial.print("Mittwoch ");
    break;

  case 4:
    Serial.print("Donnerstag ");
    break;

  case 5:
    Serial.print("Freitag ");
    break;

  case 6:
    Serial.print("Samstag ");
    break;
  }

  // Tag: führende 0 ergänzen
  if (Zeit.tm_mday < 10)
    Serial.print("0");
  Serial.print(Zeit.tm_mday);
  Serial.print(".");

  // Monat: führende 0 ergänzen
  if (Zeit.tm_mon < 10)
    Serial.print("0");
  Serial.print(Zeit.tm_mon + 1);
  Serial.print(".");

  // Anzahl Jahre seit 1900
  Serial.print(Zeit.tm_year + 1900);
  Serial.print(" ");

  if (Zeit.tm_hour < 10)
    Serial.print("0");
  Serial.print(Zeit.tm_hour);
  Serial.print(":");

  if (Zeit.tm_min < 10)
    Serial.print("0");
  Serial.print(Zeit.tm_min);
  Serial.print(":");
  if (Zeit.tm_sec < 10)
    Serial.print("0");
  Serial.print(Zeit.tm_sec);
  Serial.println();
  Serial.print("Tage seit 1. Januar: ");
  Serial.print(Zeit.tm_yday);
  Serial.println();

  // Normalzeit/Sommerzeit
  if (Zeit.tm_isdst > 0)
    Serial.println("MESZ = Mitteleuropäische Sommerzeit");
  else
    Serial.println("MEZ = Mitteleuropäische Zeit");
}