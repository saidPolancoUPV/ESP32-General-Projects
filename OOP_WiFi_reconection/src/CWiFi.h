#ifndef CWIFI_H
#define CWIFI_H

#include <Arduino.h>
#include <WiFi.h>

class CWiFi {
    private:
     String SSID;
     String PASSWORD;
     bool VERBOSE;
     unsigned long _previous_time;
     unsigned long _time_delay;
    
    public:
    CWiFi(String ssid, String password, bool verbose);
    void init();
    void reconnect();
    void set_time_delay(unsigned long td);
};

#endif