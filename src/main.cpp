#include "mbed.h"

#include "../include/light_sensor.h"
#include "../include/hum_sensor.h"
#include "../include/temp_sensor.h"
#include "../include/mqtt_manager_core.h"

#define CURRENT_MACHINE "sensor_hub"

I2C i2c(I2C_SDA, I2C_SCL);

void setup() {
    light_init();
    
    //empty for now
    humid_init();
    temp_init();
    //setup wifi here

    WiFiInterface *wifi = WiFiInterface::get_default_instance();

    mqtt_init_client(wifi, "your_ssid", "your_password", "test.mosquitto.org", 1883, CURRENT_MACHINE);

    if (CURRENT_MACHINE == "server_hub") {
        //subscribe to topics
        mqtt_subscribe("sensorhub/temperature");
        mqtt_subscribe("sensorhub/humidity");
        mqtt_subscribe("sensorhub/light");

    }
}

int main() {
    setup();

    bool sensor_hub = true;

    while (true) {
        if (sensor_hub) {
            float humidity = read_humidity();
            char buf[16];
            snprintf(buf, sizeof(buf), "%.2f", humidity);
            mqtt_publish("sensorhub/humidity", buf);

            float temperature = read_temperature(); 
            snprintf(buf, sizeof(buf), "%.2f", temperature);
            mqtt_publish("sensorhub/temperature", buf);

            float lux = read_light();
            snprintf(buf, sizeof(buf), "%.2f", lux);
            mqtt_publish("sensorhub/light", buf);

        }
        mqtt_yield(100);

        ThisThread::sleep_for(100ms);
    }
}