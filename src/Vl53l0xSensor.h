#pragma once

#include <VL53L0X.h>
#include "DistanceSensor.h"

class Vl53l0xSensor : public DistanceSensorValue {
public:
    static byte nextAddress;
    static const unsigned short MAX_DISTANCE = 150;

private:

    VL53L0X sensor = VL53L0X();
    const byte sensorPin;


public:
    Vl53l0xSensor(const byte sensorPin) : sensorPin(sensorPin) {
        initSensor();
    }

    static void lowPin(const byte sensorPin) {
        pinMode(sensorPin, OUTPUT);
        digitalWrite(sensorPin, LOW);
    }

    void initSensor() {
        digitalWrite(sensorPin, HIGH);

        sensor.setTimeout(100);
        sensor.init();
        sensor.setMeasurementTimingBudget(20000);
        sensor.startContinuous(20);

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

byte Vl53l0xSensor::nextAddress = 0x30;
