#include <Arduino.h>

#include "LSM6DSO32.h"

#define CS_PIN 10
LSM6DSO32 LSM(CS_PIN, SPI, 400000);

#define RAD_TO_DEG 180/PI

void setup() {
// write your initialization code here
    Serial.begin(115200);

    LSM.begin();

    LSM.default_configuration();

}

void loop() {
    while(LSM.get_accel_drdy_status() != 1);
    Vector<double, 3> acc = LSM.get_accel();

    Vector<double, 3> euler = {0,0,0};
    euler[0] = atan2(acc[1], acc[2]) * RAD_TO_DEG;
    euler[1] = atan2(acc[2], -acc[0]) * RAD_TO_DEG;

    Serial.printf("Euler: %f %f %f\n", euler[0], euler[1], euler[2]);
}