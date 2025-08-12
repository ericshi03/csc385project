#ifndef CORE_MQTT_CONFIG_H_
#define CORE_MQTT_CONFIG_H_

/*
 * core_mqtt_config.h
 * User configuration for coreMQTT library.
 * 
 * This file must be in your project's include path.
 * Adjust buffer sizes and options for your application.
 */

/*====================== General Settings ======================*/

/* Enable asserts (set to 0 to disable for release builds). */
#define MQTT_ENABLE_ASSERTS          1

/* Max length of MQTT client identifier. */
#define MQTT_CLIENT_IDENTIFIER_MAX_LENGTH    64

/* Max length for any topic string. */
#define MQTT_TOPIC_MAX_LENGTH                128

/* Max number of topic subscriptions tracked. */
#define MQTT_SUBSCRIPTIONS_MAX_COUNT         10

/*====================== I/O Timeouts ======================*/

/* Timeout for receiving packets (milliseconds). */
#define MQTT_RECV_POLLING_TIMEOUT_MS         1000

/* Timeout for sending packets (milliseconds). */
#define MQTT_SEND_POLLING_TIMEOUT_MS         1000

/*====================== Keep Alive ======================*/

/* Keep-alive interval (seconds). */
#define MQTT_KEEP_ALIVE_INTERVAL_SECONDS     60

/*====================== Clean Session ======================*/

/* Start with a clean session by default. */
#define MQTT_CLEAN_SESSION                   1

/*====================== Resend / Retry ======================*/

/* Number of times to retry resending QoS1 publishes. */
#define MQTT_MAX_RETRY_PUBLISH               3

/*====================== Buffer Sizes ======================*/

/* Size of the network buffer used by coreMQTT (bytes). */
#define MQTT_NETWORK_BUFFER_SIZE              1024

/*====================== Optional Logging ======================*/
/*
 * Define MQTT_DO_NOT_USE_CUSTOM_CONFIG if you want to bypass
 * this file completely (not recommended).
 * 
 * You can integrate with Mbed's logging by redefining LogError, LogInfo, etc.
 */
#define LogError( message )    printf("MQTT Error: %s\n", message)
#define LogWarn( message )     printf("MQTT Warn: %s\n", message)
#define LogInfo( message )     printf("MQTT Info: %s\n", message)
#define LogDebug( message )    printf("MQTT Debug: %s\n", message)

#endif /* CORE_MQTT_CONFIG_H_ */