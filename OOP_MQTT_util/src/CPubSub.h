#ifndef CPUBSUB_H
#define CPUBSUB_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "CWiFi.h"

class CPubSub {
    private:
        String _MQTT_SERVER;
        String _MQTT_USER;
        String _MQTT_PASSWORD;
        String _MQTT_DEVICE_NAME;

        WiFiClient _espClient;
        PubSubClient _client;
        unsigned long _lastMsg;
        int _value;
        unsigned int _mqtt_port = 1883;

        String _subscription_vec;

        void init();
        void callback(char* topic, byte* message, unsigned int length);
        void reconnect();

    public:
        CPubSub(String ssid, String password, String mqtt_server, bool verbose);
        void 
}


#endif