#include "CPubSub.h"

CPubSub::CPubSub(String ssid, String password, String mqtt_server, bool verbose=true) {

    // establecemos los valores para la red;
    this->_ssid = ssid;
    this->_wifi_password = password;

    this->_verbose = verbose;

    this->_wifi = CWiFi(this->_ssid.c_str(), this->_wifi_password.c_str(), this->_verbose);

    // establecemos los valores para el servidor mqtt
    this->_mqtt_server = mqtt_server;
    this->_mqtt_device_name = "esp32";
    this->_mqtt_user = "anonimuous";
    this->_mqtt_password = "";
    this->_mqtt_port = 1883;

    this->_list_of_tipics = NULL;
    this->_num_of_topics = 0;

    this->_client = PubSubClient(this->_espClient);
    this->_lastMsg = 0;
    this->_value = 0;
}

void CPubSub::init() {
    this->_wifi.init();
}

void CPubSub::set_subcriptions_topics(String * list_of_topics, byte num_of_topics) {
    ;
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i=0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // aquí se ponen los eventos que se quieran manejar
    /*********************** Ejemplo **********************************************************/
    /*
        if (String(topic) == "esp32/output") {
            Serial.print("Changing output to ");
            if(messageTemp == "on"){
                Serial.println("on");
                digitalWrite(ledPin, HIGH);
            } else if(messageTemp == "off"){
                Serial.println("off");
                digitalWrite(ledPin, LOW);
            }
        }
    */
    /******************************************************************************************/
}

void CPubSub::loop() {
    this->_wifi.reconnect();
    this->_client.setServer(this->_mqtt_server.c_str(), this->_mqtt_port);
    this->_client.setCallback(callback);
}

CPubSub::~CPubSub() {
    if (this->_list_of_tipics != NULL) {
        delete [] this->_list_of_tipics;
    }
}