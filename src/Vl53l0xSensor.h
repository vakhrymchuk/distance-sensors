#pragma once

#include <VL53L0X.h>
#include "DistanceSensor.h"

class Vl53l0xSensor : public DistanceSensorValue {
public:
    static const unsigned short MAX_DISTANCE = 200;

protected:

    VL53L0X sensor = VL53L0X();

public:

    virtual void initSensor() {
        sensor.setTimeout(100);
        sensor.init();
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
        if (sensor.timeoutOccurred()) Serial.println(F("VL53L0X TIMEOUT"));
#endif
        newValue /= 10;
        newValue = constrain(newValue, 0, MAX_DISTANCE);
        if (newValue > 0) value = newValue;
    }

};
