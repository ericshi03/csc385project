#include "mbed.h"
#include "light_sensor.h"
#include "main.h"


const int BH1750_ADDR = 0x23 << 1; 


void light_init() {
    char cmd[1] = {0x10}; 
    i2c.write(BH1750_ADDR, cmd, 1);
}

float read_light() {
    char data[2]; // format for BH1750 is 2 bytes for light level
    i2c.read(BH1750_ADDR, data, 2);
    int light = (data[0] << 8) | data[1];
    return light; 
}
