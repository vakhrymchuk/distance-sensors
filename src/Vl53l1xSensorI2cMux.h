#pragma once

#include "Vl53l1xSensor.h"

/**
 * Create all sensors, close all channels.
 * After that invoke initSensor method for each sensor.
 */
class Vl53l1xSensorI2cMux : public Vl53l1xSensor {
private:

    const byte channel;
    TCA9548A *i2cMux;

public:
    explicit Vl53l1xSensorI2cMux(const byte channel, TCA9548A *i2cMux) :
            channel(channel), i2cMux(i2cMux) {
    }

    void initSensor() override {
        i2cMux->openChannel(channel);
        Vl53l1xSensor::initSensor();
        i2cMux->closeChannel(channel);
    }

    unsigned short getDistance() override {
        i2cMux->openChannel(channel);
        unsigned short dist = Vl53l1xSensor::getDistance();
        i2cMux->closeChannel(channel);
        return dist;
    }

private:

};
