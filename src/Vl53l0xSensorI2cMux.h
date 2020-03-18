#pragma once

#include "Vl53l0xSensor.h"

class Vl53l0xSensorI2cMux : public Vl53l0xSensor {
private:

    const byte channel;
    TCA9548A *i2cMux;

public:
    explicit Vl53l0xSensorI2cMux(const byte channel, TCA9548A *i2cMux) :
            channel(channel), i2cMux(i2cMux) {
    }

    void initSensor() override {
        i2cMux->openChannel(channel);
        Vl53l0xSensor::initSensor();
        i2cMux->closeChannel(channel);
    }

    unsigned short getDistance() override {
        i2cMux->openChannel(channel);
        unsigned short dist = Vl53l0xSensor::getDistance();
        i2cMux->closeChannel(channel);
        return dist;
    }

private:

};
