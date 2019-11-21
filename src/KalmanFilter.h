#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "DistanceSensor.h"
//#include "ValueBase.h"

static const float KALMAN_FACTOR = 0.3;
//static ValueFloat kalmanFactorValue = ValueFloat(KALMAN_FACTOR);

class KalmanFilter : public DistanceSensor {
public:
    KalmanFilter(DistanceSensor *sensor) : sensor(sensor) {
        prevValue = sensor->getDistance();
    }

    virtual unsigned short getDistance() override {
        return prevValue = (unsigned short) (sensor->getDistance() * KALMAN_FACTOR +
                                             (1 - KALMAN_FACTOR) * prevValue);
    }

private:
    DistanceSensor *sensor;
    unsigned short prevValue = 0;
};

#endif
