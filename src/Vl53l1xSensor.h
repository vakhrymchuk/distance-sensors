#pragma once

#include <VL53L1X.h>
#include "DistanceSensor.h"

class Vl53l1xSensor : public DistanceSensorValue {
public:
    unsigned short maxDistance = 400;

protected:

    VL53L1X sensor = VL53L1X();

public:

    virtual void initSensor() override {
        sensor.setTimeout(100);
        if (!sensor.init()) {
            Serial.println(F("Failed to detect and initialize sensor VL53L1X!"));
            delay(100);
        }
        sensor.setDistanceMode(VL53L1X::Medium);
        sensor.setMeasurementTimingBudget(33000);
        sensor.startContinuous(33);
        maxDistance = 320;
    }

    unsigned short getDistance() override {
        readNewValue();
        return value;
    }

private:
    void readNewValue() {
        unsigned short newValue = sensor.read(false);
#ifdef DEBUG
        if (sensor.timeoutOccurred()) Serial.print('!');
#endif
        newValue /= 10;
        newValue = constrain(newValue, 0, maxDistance);
        if (newValue > 0) value = newValue;
    }

};
