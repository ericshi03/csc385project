#include <mbed.h>
#include "WiFiInterface.h"
extern "C" {
    #include "core_mqtt.h"
}

//i hate m life


const char *SSID = "YourWiFiSSID";
const char *password = "YourWiFiPassword";

// MQTT Broker details
const char *mqtt_broker = "mqtt.eclipse.org";  
const int mqtt_port = 1883;
const char *mqtt_client_id = "STM32_MQTT_Client";
const char *mqtt_topic = "test/topic";

WiFiInterface *wifi;  

// MQTT connection variables
MQTTContext_t mqtt_context;
//NetworkContext_t network_context;


