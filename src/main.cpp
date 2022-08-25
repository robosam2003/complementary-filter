#include <Arduino.h>

#include "LSM6DSO32.h"

#define CS_PIN 10
LSM6DSO32 LSM(CS_PIN, SPI, 400000);

void setup() {
// write your initialization code here
    Serial.begin(115200);

    LSM.begin();

    LSM.default_configuration();

}

void loop() {
    while(LSM.get_accel_drdy_status() != 1);
    Vector<double, 3> acc = LSM.get_accel();


    Serial.printf("Acc: %lf, %lf, %lf\n", acc[0], acc[1], acc[2]);
}