#pragma once

#include "Vl53l0xSensor.h"

/**
 * Create all sensors, it will low all pins.
 * After that invoke initSensor method for each sensor.
 */
class Vl53l0xSensorPin : public Vl53l0xSensor {
public:
    static byte nextAddress;

private:

    const byte sensorPin;
    const byte sensorAddress;

public:
    explicit Vl53l0xSensorPin(const byte sensorPin, VL53Mode mode = NORMAL) : Vl53l0xSensor(mode),
            sensorPin(sensorPin), sensorAddress(nextAddress++) {
        lowPin();
    }

    void initSensor() override {
#ifdef DEBUG
        Serial.print(F("Init sensor on pin: "));
        Serial.println(sensorPin);
#endif

        digitalWrite(sensorPin, HIGH);
        Vl53l0xSensor::initSensor();

#ifdef DEBUG
        Serial.print(F("Use address: "));
        Serial.println(sensorAddress);
#endif
        sensor.setAddress(sensorAddress);
    }

private:
    void lowPin() const {
        pinMode(sensorPin, OUTPUT);
        digitalWrite(sensorPin, LOW);
    }

};

byte Vl53l0xSensorPin::nextAddress = 0x30;
