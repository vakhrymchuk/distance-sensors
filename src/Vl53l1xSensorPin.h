#pragma once

#include "Vl53l1xSensor.h"

/**
 * Create all sensors, it will low all pins.
 * After that invoke initSensor method for each sensor.
 */
class Vl53l1xSensorPin : public Vl53l1xSensor {
public:
    static byte nextAddress;

private:

    const byte sensorPin;

public:
    explicit Vl53l1xSensorPin(const byte sensorPin) :
            sensorPin(sensorPin) {
        lowPin();
    }

    void initSensor() override {
        digitalWrite(sensorPin, HIGH);
        Vl53l1xSensor::initSensor();

#ifdef DEBUG
        Serial.print(F("Use address: "));
        Serial.println(nextAddress);
#endif
        sensor.setAddress(nextAddress++);
    }

private:
    void lowPin() const {
        pinMode(sensorPin, OUTPUT);
        digitalWrite(sensorPin, LOW);
    }

};

byte Vl53l1xSensorPin::nextAddress = 0x50;
