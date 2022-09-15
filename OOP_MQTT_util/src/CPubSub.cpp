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

    this->_list_of_tipics = new String[1];
    this->_num_of_topics = 1;

    this->_client = PubSubClient(this->_espClient);
    this->_lastMsg = 0;
    this->_value = 0;
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

void CPubSub::init() {
    this->_wifi.init();
    delay(500);
    this->_list_of_tipics[0] = String("/public");
    this->_client.setServer(this->_mqtt_server.c_str(), this->_mqtt_port);
    this->_client.setCallback(callback);
}

void CPubSub::set_subcriptions_topics(String * list_of_topics, byte num_of_topics) {
    delete [] this->_list_of_tipics;
    this->_list_of_tipics = new String[num_of_topics];

    for (byte i=0; i < num_of_topics; i++) {
        this->_list_of_tipics[i] = list_of_topics[i];
    }
}

void CPubSub::loop() {
    this->_wifi.reconnect();
    this->reconnect();
}

void CPubSub::reconnect() {
    while (!this->_client.connected()) {
        Serial.print("Conectando al servidor MQTT...");
        if (this->_client.connect(this->_mqtt_device_name.c_str(), this->_mqtt_user.c_str(), this->_mqtt_password.c_str())) {
            Serial.println("Connected.");
            delay(1000);
            /* for (byte i=0; i < this->_num_of_topics; i++) {
                this->_client.subscribe(this->_list_of_tipics[i].c_str());
                delay(500);
            } */
            this->_client.subscribe("/esp32/test");
            Serial.println("**** Done ***");
        } else {
            Serial.print("Error en la conexión, rc=");
            Serial.print(this->_client.state());
            Serial.println(" intentando en 5 segundos");
            delay(5000);
        }
    }
}

void CPubSub::set_mqtt_user(String user_name) {
    this->_mqtt_user = user_name;
}

void CPubSub::set_mqtt_password(String passwd) {
    this->_mqtt_password = passwd;
}

CPubSub::~CPubSub() {
    if (this->_list_of_tipics != NULL) {
        delete [] this->_list_of_tipics;
    }
}