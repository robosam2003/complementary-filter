#include <Arduino.h>

#include "LSM6DSO32.h"
#include "RCFilter.h"


#define CS_PIN 10
LSM6DSO32 LSM(CS_PIN, SPI, 400000);

#define RAD_TO_DEG 180/PI

void setup() {
// write your initialization code here
    Serial.begin(115200);

    LSM.begin();

    LSM.default_configuration();
    LSM.set_accel_ODR(OUTPUT_DATA_RATES::ODR_104_HZ);
    LSM.set_gyro_ODR(OUTPUT_DATA_RATES::ODR_104_HZ);
    LSM.set_accel_high_pass_or_LPF2_filter_cutoff(ACCEL_HP_OR_LPF2_CUTOFF::ODR_OVER_100);

}


// everything in radians for calc.
Vector<double, 3> eul_hat = {0,0,0};
double alpha = 0.01;
unsigned long prevTime_us = 0;
void loop() {
    // Accelerometer estimation - assume body is not accelerating.
    while(LSM.get_accel_drdy_status() == 0);
    Vector<double, 3> acc = LSM.get_accel();
    Vector<double, 3> gyr = LSM.get_gyro();
    gyr *= DEG_TO_RAD;
    Vector<double, 3> euler_hat_a = {0,0,0};


    euler_hat_a[0] = atan2(acc[1], acc[2]);
    euler_hat_a[1] = atan2(-acc[0], sqrt(pow(acc[1], 2) + pow(acc[2], 2)));

    // Gyroscope
    //while(LSM.get_gyr_drdy_status() == 0);

    Vector<double, 3> eul_rate_g = {0,0,0};

    eul_rate_g[0] = gyr[0] + gyr[1]*sin(eul_hat[0])*tan(eul_hat[1]) + gyr[2]*cos(eul_hat[0])*tan(eul_hat[1]);
    eul_rate_g[1] = 0      + gyr[1]*cos(eul_hat[0])                 + gyr[2]*(-sin(eul_hat[0]));


    // Complementary filter
    unsigned long currentTime_us = micros();
    eul_hat[0] = euler_hat_a[0]*alpha  +  (1-alpha)*(eul_hat[0] + (1e-6)*(currentTime_us-prevTime_us)*eul_rate_g[0]);
    eul_hat[1] = euler_hat_a[1]*alpha  +  (1-alpha)*(eul_hat[1] + (1e-6)*(currentTime_us-prevTime_us)*eul_rate_g[1]);
    prevTime_us = currentTime_us;

    Serial.printf("EUL_HAT: %lf, %lf, %lf\n", eul_hat[0]*RAD_TO_DEG, eul_hat[1]*RAD_TO_DEG, eul_hat[2]*RAD_TO_DEG);
}