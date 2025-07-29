#include "mbed.h"

#include "../include/light_sensor.h"
#include "../include/hum_sensor.h"
#include "../include/temp_sensor.h"


void setup() {
    light_init();
    humid_init();
}

int main() {
    setup();

    bool swap_light_humidity = true;

    while (true) {
        if (swap_light_humidity) {
            float humidity = read_humidity();
            printf("Humidity sensor (SHT31) humidity: %.2f%%\n", humidity);

            float temperature = read_temperature(); 
            printf("Temperature sensor (SHT31) temperature: %.2f°C\n", temperature); 
        } else {
            float lux = read_light();
            printf("Light sensor (BH1750) lux: %.2f\n", lux);
        }




        ThisThread::sleep_for(500ms);
    }
}