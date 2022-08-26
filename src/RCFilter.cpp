//
// Created by robosam2003 on 26/08/2022.
//
#include "RCFilter.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif // PI

RCFilter::RCFilter(double cuttoff_freq) {
    RC = 1 / (2 * PI * cuttoff_freq);
    prev_value = 0;
    prev_update_time_us = 0;
}

double RCFilter::update(double input, unsigned long current_time_us) {
    double T = (current_time_us - prev_update_time_us) * 1e-6;
    prev_update_time_us = current_time_us;

    double coeff1 = T / (T + RC);
    double coeff2 = RC / (T + RC);

    double out = input*coeff1 + prev_value*coeff2;
    prev_value = out;
    return out;
}









// copy constructor
RCFilter::RCFilter(const RCFilter &other) {
    RC = other.RC;
    prev_value = other.prev_value;
    prev_update_time_us = other.prev_update_time_us;
}

// copy assignment operator
RCFilter &RCFilter::operator=(const RCFilter &other) {
    if (this != &other) {
        RC = other.RC;
        prev_value = other.prev_value;
        prev_update_time_us = other.prev_update_time_us;
    }
    return *this;
}

// move constructor
RCFilter::RCFilter(RCFilter &&other) {
    RC = other.RC;
    prev_value = other.prev_value;
    prev_update_time_us = other.prev_update_time_us;
}

// move assignment operator
RCFilter &RCFilter::operator=(RCFilter &&other) {
    if (this != &other) {
        RC = other.RC;
        prev_value = other.prev_value;
        prev_update_time_us = other.prev_update_time_us;
    }
    return *this;
}



