#include "mqtt_manager_core.h"
#include <string.h>

// Global/static variables for the MQTT connection
static NetworkContext_t networkContext;
static MQTTContext_t mqttContext;
static MQTTFixedBuffer_t mqttBuffer;
static uint8_t bufferArray[ 1024 ]; // MQTT TX/RX buffer

static WiFiInterface *wifiInterface = nullptr;
static TCPSocket socket;

#define MQTT_BROKER_ADDRESS "test.mosquitto.org"
#define MQTT_BROKER_PORT 1883


uint32_t getTimeMs()
{
    return Kernel::Clock::now().time_since_epoch().count() / 1000000; // or any mbed timer in ms
}

static int32_t transport_send( NetworkContext_t *pNetworkContext,
                               const void *pBuffer,
                               size_t bytesToSend )
{
    return pNetworkContext->socket->send( pBuffer, bytesToSend );
}

static int32_t transport_recv( NetworkContext_t *pNetworkContext,
                               void *pBuffer,
                               size_t bytesToRecv )
{
    return pNetworkContext->socket->recv(pBuffer, bytesToRecv);
}

static TransportInterface_t transportInterface = {
    transport_recv,    
    transport_send,    
    NULL,             
    &networkContext  
};

bool mqtt_init_client(WiFiInterface *wifi,
                       const char *ssid,
                       const char *password,
                       const char *broker,
                       int port,
                       const char *clientId)
{
    wifiInterface = wifi;

    printf("Connecting to WiFi: %s...\n", ssid);
    if( wifiInterface->connect(ssid, password, NSAPI_SECURITY_WPA_WPA2) != 0 )
    {
        printf("WiFi connection failed!\n");
        return false;
    }
    printf("WiFi connected\n");

    printf("Connecting to MQTT broker: %s:%d...\n", broker, port);
    SocketAddress sockAddr;
    nsapi_error_t result = wifi->gethostbyname(MQTT_BROKER_ADDRESS, &sockAddr);
    sockAddr.set_port(MQTT_BROKER_PORT);

    nsapi_error_t socket_status = socket.open(wifiInterface);
    if( socket.connect(sockAddr) != 0 )
    {
        printf("Socket connection failed!\n");
        return false;
    }

    networkContext.socket = &socket;

    mqttBuffer.pBuffer = bufferArray;
    mqttBuffer.size = sizeof( bufferArray );

MQTTStatus_t status = MQTT_Init(
    &mqttContext,
    &transportInterface,
    getTimeMs,
    NULL,      
    &mqttBuffer       
);

    if( status != MQTTSuccess )
    {
        printf("MQTT_Init failed: %d\n", status);
        return false;
    }

    MQTTConnectInfo_t connectInfo = { 0 };
    connectInfo.cleanSession = true;
    connectInfo.pClientIdentifier = clientId;
    connectInfo.clientIdentifierLength = strlen( clientId );
    connectInfo.keepAliveSeconds = 60;

    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 5000, NULL );
    if( status != MQTTSuccess )
    {
        printf("MQTT_Connect failed: %d\n", status);
        return false;
    }

    printf("MQTT connected!\n");
    return true;
}

//--------------------------------------------------
// Publish message
//--------------------------------------------------
bool mqtt_publish( const char *topic, const char *message )
{
    MQTTPublishInfo_t publishInfo;
    publishInfo.qos = MQTTQoS0;
    publishInfo.pTopicName = topic;
    publishInfo.topicNameLength = strlen( topic );
    publishInfo.pPayload = message;
    publishInfo.payloadLength = strlen( message );

    MQTTStatus_t status = MQTT_Publish( &mqttContext, &publishInfo, 0 );
    if( status != MQTTSuccess )
    {
        printf("MQTT_Publish failed: %d\n", status);
        return false;
    }

    return true;
}

//--------------------------------------------------
// Process incoming MQTT packets
//--------------------------------------------------
void mqtt_yield(int timeoutMs)
{
    uint32_t start = getTimeMs();
    while ((getTimeMs() - start) < (uint32_t)timeoutMs)
    {
        MQTT_ProcessLoop(&mqttContext);
        ThisThread::sleep_for(10ms);
    }
}

//--------------------------------------------------
// Disconnect MQTT
//--------------------------------------------------
void mqtt_disconnect()
{
    MQTT_Disconnect( &mqttContext );
    socket.close();
    wifiInterface->disconnect();
}