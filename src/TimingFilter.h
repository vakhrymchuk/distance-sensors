#pragma once

#include "DistanceSensor.h"
#include "Stopwatch.h"

// http://umath.ru/calc/graph/?&point=-247;61&scale=5;2&func=round(x%5E0.5);
class TimingFilter : public DistanceSensor {
public:

    TimingFilter(DistanceSensor *sensor) : sensor(sensor) {}

    unsigned short getDistance() override final {
        unsigned short distance = sensor->getDistance();

        return updateValue(distance);
    }

    bool isLongerThan(const unsigned long ms) const {
        return stopwatch->isMoreThan(ms);
    }

    unsigned short updateValue(unsigned short distance) {
        byte value = (byte) sqrt(distance);
        if (lastValue != value) {
            lastValue = value;
            stopwatch->start();
        }
        return distance;
    }

private:
    DistanceSensor *sensor;

    byte lastValue;

    Stopwatch *stopwatch = new Stopwatch();


};
