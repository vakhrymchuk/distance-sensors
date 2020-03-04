#pragma once

#include <VL53L1X.h>
#include "DistanceSensor.h"

class Vl53l1xSensor : public DistanceSensorValue {
public:
    static byte nextAddress;
    static const unsigned short MAX_DISTANCE = 400;

private:

    VL53L1X sensor = VL53L1X();
    const byte sensorPin;


public:
    Vl53l1xSensor(const byte sensorPin) : sensorPin(sensorPin) {
        initSensor();
    }

    static void lowPin(const byte sensorPin) {
        pinMode(sensorPin, OUTPUT);
        digitalWrite(sensorPin, LOW);
    }

    void initSensor() {
        digitalWrite(sensorPin, HIGH);

        sensor.init();

        sensor.setTimeout(500);
        sensor.setDistanceMode(VL53L1X::Long);
        sensor.setMeasurementTimingBudget(50000);
        sensor.startContinuous(50);

#ifdef DEBUG
        Serial.print(F("Use address: "));
        Serial.println(nextAddress);
#endif
        sensor.setAddress(nextAddress++);
    }

    unsigned short getDistance() override {
#ifdef DEBUG
        if (sensor.timeoutOccurred()) Serial.print(F(" TIMEOUT"));
#endif
        unsigned short newValue = readNewValue();
        if (newValue > 0) {
            value = newValue;
        }
        return value;
    }

    unsigned short readNewValue() {
        unsigned short newValue = sensor.readRangeContinuousMillimeters();
        newValue /= 10;
        newValue = constrain(newValue, 0, MAX_DISTANCE);
        return newValue;
    }

};

byte Vl53l1xSensor::nextAddress = 0x40;
