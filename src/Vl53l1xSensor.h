#pragma once

#include <VL53L1X.h>
#include "DistanceSensor.h"

class Vl53l1xSensor : public DistanceSensorValue {
public:
    static const unsigned short MAX_DISTANCE = 400;

protected:

    VL53L1X sensor = VL53L1X();


public:

    virtual void initSensor() {
        sensor.setTimeout(100);
        sensor.init();
        sensor.setDistanceMode(VL53L1X::Short);
        sensor.setMeasurementTimingBudget(20000);
        sensor.startContinuous(20);
    }

    unsigned short getDistance() override {
        readNewValue();
        return value;
    }

private:
    void readNewValue() {
        unsigned short newValue = sensor.readRangeContinuousMillimeters();
#ifdef DEBUG
        if (sensor.timeoutOccurred()) Serial.println(F("VL53L1X TIMEOUT"));
#endif
        newValue /= 10;
        newValue = constrain(newValue, 0, MAX_DISTANCE);
        if (newValue > 0) value = newValue;
    }

};
