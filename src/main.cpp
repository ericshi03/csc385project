#include "mbed.h"

#include "../include/light_sensor.h"
#include "../include/hum_sensor.h"
#include "../include/temp_sensor.h"
#include "../include/mqtt_manager.h"



void setup() {
    light_init();
    
    //empty for now
    humid_init();
    temp_init();
    mqtt_init();
}

int main() {
    setup();

    bool sensor_hub = true;

    while (true) {
        if (sensor_hub) {
            float humidity = read_humidity();
            char buf[16];
            snprintf(buf, sizeof(buf), "%.2f", humidity);
            publish_message("sensorhub/humidity", buf);

            float temperature = read_temperature(); 
            char buf[16];
            snprintf(buf, sizeof(buf), "%.2f", temperature);
            publish_message("sensorhub/temperature", buf);

            float lux = read_light();
            char buf[16];
            snprintf(buf, sizeof(buf), "%.2f", lux);
            publish_message("sensorhub/lux", buf);
        }

        mqtt_yield(100);

        ThisThread::sleep_for(500ms);
    }
}