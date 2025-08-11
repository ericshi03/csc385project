#include "mbed.h"
#include "MQTTClientMbedOs.h"
#include "WiFiInterface.h"


#define MQTT_BROKER_ADDRESS "test.mosquitto.org"
#define MQTT_BROKER_PORT 1883

const char* WIFI_SSID = "your_ssid";
const char* WIFI_PASSWORD = "your_password";

WiFiInterface *wifi;
static TCPSocket socket;
static MQTTClient client(&socket);


bool connect_wifi() {
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("No WiFi interface found\n");
        return false;
    }

    nsapi_error_t status = wifi->connect(WIFI_SSID, WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (status != NSAPI_ERROR_OK) {
        printf("WiFi connection failed: %d\n", status);
        return false;
    }
    
    printf("Connected to WiFi network: %s\n", WIFI_SSID);
    return true;
}

bool connect_mqtt() {
    SocketAddress sockAddr;
    nsapi_error_t result = wifi->gethostbyname(MQTT_BROKER_ADDRESS, &sockAddr);
    if (result != NSAPI_ERROR_OK) {
        return false;
    }
    sockAddr.set_port(MQTT_BROKER_PORT);

    nsapi_error_t socket_status = socket.open(wifi);
    if (socket_status != NSAPI_ERROR_OK) {
        return false;
    }
    int rc = socket.connect(sockAddr);
    if (rc != 0) {
        printf("Socket connect failed: %d\n", rc);
        return false;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = (char *)"mbed_client";

    rc = client.connect(connectData);
    if (rc != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return false;
    }

    // Optional: publish a test message
    const char* topic = "mbed/test";
    const char* payload = "Hello from DISCO_L475VG_IOT01A";

    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    rc = client.publish(topic, message);
    if (rc == 0) {
        printf("Published message: %s to topic: %s\n", payload, topic);
    } else {
        printf("Failed to publish message: %d\n", rc);
    }

    return true;
}

void publish_message(char * topic, char * message) {
    MQTT::Message msg;
    msg.qos = MQTT::QOS0;
    msg.retained = false;
    msg.dup = false;
    msg.payload = (void*)message;
    msg.payloadlen = strlen(message);

    int rc = client.publish(topic, msg);
    if (rc != 0) {
        printf("Failed to publish message: %d\n", rc);
    } else {
        printf("Message published successfully: %s\n", message);
    }

}

bool subscribe_to_topic(const char* topic) {
    int rc = client.subscribe(topic, MQTT::QOS1, [](MQTT::MessageData& md) {
        MQTT::Message &message = md.message;
        printf("Received message: %.*s on topic: %.*s\n", 
               (int)message.payloadlen, (char*)message.payload,
               (int)md.topicName.lenstring.len, md.topicName.lenstring.data);
    });
    
    if (rc != 0) {
        printf("Failed to subscribe to topic %s: %d\n", topic, rc);
        return false;
    }
    printf("Subscribed to topic: %s\n", topic);
    return true;
}
      
void mqtt_init() {
    if (connect_wifi()) {
        printf("WiFi connected successfully\n");
    } else {
        printf("Failed to connect to WiFi\n");
        return;
    }

    if (connect_mqtt()) {
        printf("MQTT connected successfully\n");
    } else {
        printf("Failed to connect to MQTT broker\n");
        return; 
    }

    if(subscribe_to_topic("sensorhub/temperature")) {
        printf("Subscribed to temperature topic\n");
    } else {
        printf("Failed to subscribe to temperature topic\n");
    }

    if(subscribe_to_topic("sensorhub/humidity")) {
        printf("Subscribed to humidity topic\n");
    } else {
        printf("Failed to subscribe to humidity topic\n");
    }
    if (subscribe_to_topic("sensorhub/light")) {
        printf("Subscribed to light topic\n");
    } else {
        printf("Failed to subscribe to light topic\n");
    }
}

void disconnect() {
    if (client.isConnected()) {
        client.disconnect();
        socket.close();
        wifi->disconnect();
        printf("Disconnected from MQTT broker and WiFi\n");
    } else {
        printf("Client is not connected to MQTT broker\n");
    }
}


void mqtt_yield(int timeout_ms) {
    client.yield(timeout_ms);
}


