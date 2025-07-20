#include "mbed.h"

#include "include/light_sensor.h"


void setup() {
    bh1750_init();
    
}

int main() {
    setup();

    while (true) {
        float lux = bh1750_read();
        printf("Light sensor (BH1750) lux: %.2f\n", lux);
        ThisThread::sleep_for(500ms);
    }
}