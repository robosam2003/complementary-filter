//
// Created by robosam2003 on 26/08/2022.
//

#ifndef RCFILTER_H
#define RCFILTER_H


class RCFilter {
public:
    double RC; //equal to R*C
    double prev_value;
    unsigned long prev_update_time_us;

    explicit RCFilter(double cuttoff_freq);

    double update(double input, unsigned long current_time_us);

    // copy constructors
    RCFilter(const RCFilter& other);
    RCFilter& operator=(const RCFilter& other);

    // move constructors
    RCFilter(RCFilter&& other) noexcept ;
    RCFilter& operator=(RCFilter&& other) noexcept ;
};

#endif //RCFILTER_H
