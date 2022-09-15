#ifndef CPUBSUB_H
#define CPUBSUB_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "CWiFi.h"

class CPubSub {
    private:

        bool _verbose;

        String _ssid;
        String _wifi_password;

        CWiFi _wifi;

        String _mqtt_server;
        String _mqtt_user;
        String _mqtt_password;
        String _mqtt_device_name;
        unsigned int _mqtt_port;

        String * _list_of_tipics;
        byte _num_of_topics;

        WiFiClient _espClient;
        PubSubClient _client;
        unsigned long _lastMsg;
        int _value;
        void reconnect();

    public:
        CPubSub(String ssid, String password, String mqtt_server, bool verbose);
        void 
        void init();
        friend void callback(char* topic, byte* message, unsigned int length);
        void set_subcriptions_topics(String * list_of_topics, byte num_of_topics);
        void loop();
        ~CPubSub();
        
};


#endif