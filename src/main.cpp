#include "Arduino.h"

// Eigen
#include "eigen.h"
#include "Eigen/Core"
#include "Eigen/Geometry"

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
    LSM.set_accel_ODR(OUTPUT_DATA_RATES::ODR_1667_HZ);
    LSM.set_gyro_ODR(OUTPUT_DATA_RATES::ODR_1667_HZ);
    LSM.set_accel_high_pass_or_LPF2_filter_cutoff(ACCEL_HP_OR_LPF2_CUTOFF::ODR_OVER_100);

}


Eigen::Quaterniond q(1, 0, 0, 0);
double alpha = 0.01;
unsigned long prevTime_us = 0;
void loop() {


    // Accelerometer estimation - assume body is not accelerating.
    while(LSM.get_accel_drdy_status() == 0);
    Vector<double, 3> acc = LSM.get_accel();
    Vector<double, 3> gyr = LSM.get_gyro(); gyr*=DEG_TO_RAD;

    // Find orientation according to the accelerometer
    Eigen::Quaterniond q_body_a(q.w(), acc[0], acc[1], acc[2]);
    q_body_a.normalize();

    // Find orientation according to the gyroscope
    unsigned long currentTime_us = micros();
    double theta = gyr.norm() * (currentTime_us - prevTime_us)*(1e-6);
    prevTime_us = currentTime_us;

    Vector<double, 3> v = gyr.normalized();

    Eigen::Quaterniond q_delta_ori_g( cos(theta/2), v[0]*sin(theta/2), v[1]*sin(theta/2), v[2]*sin(theta/2) );
    Eigen::Quaterniond q_body_g = q*q_delta_ori_g;
    q_delta_ori_g.normalize();
    if (std::isnan(q_delta_ori_g.w())) {
        q_delta_ori_g.w() = 1;
        q_delta_ori_g.x() = 0;
        q_delta_ori_g.y() = 0;
        q_delta_ori_g.z() = 0;
    }


    // Find orientation according to the accelerometer and gyroscope
    Eigen::Quaterniond q_body = q_body_g.slerp(alpha, q_body_a);
    q_body.normalize();
    q = q_body;



    // /r/n = CR/LF for MATLAB
    Serial.printf("%lf, %lf, %lf, %lf\r\n", q.w(), q.x(), q.y(), q.z());
    delay(5);

    // Quaternions are beautiful.
}

