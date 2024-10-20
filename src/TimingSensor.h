#pragma once

#include "DistanceSensor.h"
#include "Stopwatch.h"

/**
 * Calculates time for the identical distance within a deviation (12.5%).
 */
class TimingSensor : public DistanceSensor {
public:
    static const int DEVIATION_SHIFT = 3;

private:
    DistanceSensor *sensor;
    unsigned short baseDistance = 0;
    Stopwatch *stopwatch = new Stopwatch();
    const byte deviationShift;

public:

    explicit TimingSensor(DistanceSensor *sensor, const byte deviationShift = DEVIATION_SHIFT) :
            sensor(sensor), deviationShift(deviationShift) {}

    unsigned short getDistance() override {
        unsigned short distance = sensor->getDistance();
        return updateValue(distance);
    }

    bool isLongerThan(const unsigned long ms) const {
        return stopwatch->isMoreThan(ms);
    }

    unsigned long time() const {
        return stopwatch->time();
    }

    DistanceSensor *getSensor() {
        return sensor;
    }

    void initSensor() override {
        sensor->initSensor();
    }

protected:


    unsigned short updateValue(unsigned short distance) {
        short diff = distance - baseDistance;
        short deviation = baseDistance >> deviationShift;
        if (abs(diff) >= deviation) {
            baseDistance = distance;
            stopwatch->start();
        }
        return distance;
    }
};
