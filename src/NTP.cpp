/**
 * @file NTP.cpp
 * @brief Test Zeit über NTP auf ESP32
 * @version 1.0
 * @date 1 Dez 2024
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2024 B. Borys
 */
#define _USE_NTP_
#ifdef _USE_NTP_
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <NTP.h>
// NTP-Server aus dem Pool: #define Zeitserver "de.pool.ntp.org"
// oder eigener Server:
#define Zeitserver "192.168.1.1"
//#define Zeitzone "CET-1CEST,M3.5.0/02,M10.5.0/03"
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);
void setup()
{
    int i = 0;
    Serial.begin(115200);
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
    ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);
    ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);
    ntp.updateInterval(600000L); // 5 Min
    ntp.begin(Zeitserver);
    ntp.update();
    WiFi.disconnect(true);
}
void loop()
{
    Serial.println(ntp.formattedTime("NTP: %d. %B %Y")); // dd. Mmm yyyy
    Serial.println(ntp.formattedTime("NTP: %A %T"));     // Www hh:mm:ss
    delay(10000);
}
#endif