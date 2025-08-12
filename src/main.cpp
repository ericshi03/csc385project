#include "mbed.h"

#include "../include/light_sensor.h"
#include "../include/hum_sensor.h"
#include "../include/temp_sensor.h"
#include "../include/mqtt_manager.h"

I2C i2c(I2C_SDA, I2C_SCL);

void setup() {
    light_init();
    
    //empty for now
    humid_init();
    temp_init();
}

int main() {
    setup();

    bool sensor_hub = true;

    while (true) {
        if (sensor_hub) {
            float humidity = read_humidity();
            char buf[16];
            snprintf(buf, sizeof(buf), "%.2f", humidity);

            float temperature = read_temperature(); 
            snprintf(buf, sizeof(buf), "%.2f", temperature);

            float lux = read_light();
            snprintf(buf, sizeof(buf), "%.2f", lux);

        }

        ThisThread::sleep_for(500ms);
    }
}