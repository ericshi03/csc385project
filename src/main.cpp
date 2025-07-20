#include "mbed.h"

#include "include/light_sensor.h"


void setup() {
    light_init();

}

int main() {
    setup();

    while (true) {
        float lux = read_light();
        printf("Light sensor (BH1750) lux: %.2f\n", lux);
        ThisThread::sleep_for(500ms);
    }
}