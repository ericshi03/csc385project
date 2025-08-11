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
            printf("Humidity sensor (SHT31) humidity: %.2f%%\n", humidity);

            float temperature = read_temperature(); 
            printf("Temperature sensor (SHT31) temperature: %.2f°C\n", temperature); 

            float lux = read_light();
            printf("Light sensor (BH1750) lux: %.2f\n", lux);
        }




        ThisThread::sleep_for(500ms);
    }
}