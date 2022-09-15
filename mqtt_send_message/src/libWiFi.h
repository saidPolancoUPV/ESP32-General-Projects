#ifndef LIBWIFI_H
#define LIBWIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include "common.h"

void initWiFi(const char * ssid, const char * passwd);
void reconexion();

#endif