#pragma once

#include <VL53L0X.h>
#include "DistanceSensor.h"

enum VL53Mode {
    NORMAL = 0, LONG_RANGE = 1, HIGH_SPEED = 2, HIGH_ACCURACY = 3
};

class Vl53l0xSensor : public DistanceSensorValue {
protected:

    VL53L0X sensor = VL53L0X();
    VL53Mode mode;
    unsigned short maxDistance = 200;

public:

    explicit Vl53l0xSensor(VL53Mode mode = NORMAL) : mode(mode) {}

    void setMode(VL53Mode mode) {
        Vl53l0xSensor::mode = mode;
        initSensor();
    }

    virtual void initSensor() {
        sensor.setTimeout(100);
        if (!sensor.init()) {
#ifdef DEBUG
            Serial.println(F("Failed to detect and initialize sensor VL53L0X!"));
            delay(10);
#endif
            return;
        }

        switch (mode) {
            case NORMAL:
                sensor.startContinuous();
                break;
            case LONG_RANGE:
                // lower the return signal rate limit (default is 0.25 MCPS)
                sensor.setSignalRateLimit(0.1);
                // increase laser pulse periods (defaults are 14 and 10 PCLKs)
                sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
                sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
                sensor.startContinuous();
                break;
            case HIGH_SPEED:
                // reduce timing budget to 20 ms (default is about 33 ms)
                sensor.setMeasurementTimingBudget(20000);
                sensor.startContinuous(20);
                maxDistance = 120;
                break;
            case HIGH_ACCURACY:
                // increase timing budget to 200 ms
                sensor.setMeasurementTimingBudget(200000);
                sensor.startContinuous();
                break;
        }

    }

    unsigned short getDistance() override {
        readNewValue();
        return value;
    }

private:
    void readNewValue() {
        unsigned short newValue = sensor.readRangeContinuousMillimeters();
        if (sensor.timeoutOccurred()) {
#ifdef DEBUG
            Serial.print('!');
#endif
            initSensor();
        }

        newValue /= 10;
        newValue = constrain(newValue, 0, maxDistance);
        if (newValue > 0) value = newValue;
    }

};
