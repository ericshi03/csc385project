#pragma once

#include <mbed.h>
#include "WiFiInterface.h"
extern "C" {
    #include "core_mqtt.h"
}

/**
 * NetworkContext_t is required by coreMQTT. We define it as a simple wrapper
 * around mbed's TCPSocket so coreMQTT knows how to send/receive data.
 */
typedef struct NetworkContext
{
    TCPSocket *socket;
} NetworkContext_t;


bool mqtt_init_client( WiFiInterface *wifi,
                       const char *ssid,
                       const char *password,
                       const char *broker,
                       int port,
                       const char *clientId );

/**
 * Publish a message to a topic.
 */
bool mqtt_publish( const char *topic, const char *message );

bool mqtt_subscribe( const char *topic);


void mqtt_yield( int timeoutMs );


void mqtt_disconnect();