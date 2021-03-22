#pragma once

#include <VL53L0X.h>
#include "DistanceSensor.h"

class Vl53l0xSensor : public DistanceSensorValue {
public:
    unsigned short maxDistance = 200;

protected:

    VL53L0X sensor = VL53L0X();

public:

    virtual void initSensor() {
        sensor.setTimeout(100);
        if (!sensor.init()) {
#ifdef DEBUG
            Serial.println(F("Failed to detect and initialize sensor VL53L0X!"));
            delay(100);
#endif
            return;
        }
        sensor.setMeasurementTimingBudget(20000);
        sensor.startContinuous(20);
        maxDistance = 120;
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
//            initSensor();
        }

        newValue /= 10;
        newValue = constrain(newValue, 0, maxDistance);
        if (newValue > 0) value = newValue;
    }

};
