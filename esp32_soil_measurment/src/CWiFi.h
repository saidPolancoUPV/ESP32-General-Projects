#ifndef CWIFI_H
#define CWIFI_H

#include <Arduino.h>
#include <WiFi.h>

class CWiFi {

    private:
        const char *ssid;
        const char *passwd;
        unsigned long previous_time;
        unsigned long time_delay;
    public:
        CWiFi(const char * ssid, const char * passwd);
        void init();
        void reconexion();
};

#endif