#pragma once

#include <Arduino.h>
#include "DistanceSensor.h"
#include "Interval.h"

class Ultrasonic : public DistanceSensor {

public:
    /** Speed of the wave: 1.0 / 29 / 2 */
    static constexpr float ULTRASONIC_FACTOR = (float) (0.5 / 29);

    static const int SAFE_DELAY = 60;

    static const unsigned short MEASUREMENT_TIMEOUT_2M_US = 23200;
    static const unsigned short MEASUREMENT_TIMEOUT_3M_US = 34000;

    Ultrasonic(byte trig, byte echo, unsigned short timeoutUs = MEASUREMENT_TIMEOUT_3M_US) :
            trig(trig), echo(echo), timeoutUs(timeoutUs) {
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        interval = new Interval(SAFE_DELAY);
    }

    unsigned short getDistance() override {
        if (interval->isReady()) {
            distance = (unsigned short) (timing() * ULTRASONIC_FACTOR);
        }
        return distance;
    }

private:

    const byte trig, echo;

    unsigned short distance = 300;

    Interval *interval;

    unsigned short timeoutUs;

    unsigned long timing() const {
        digitalWrite(trig, LOW);
        delayMicroseconds(2);

        digitalWrite(trig, HIGH);
        delayMicroseconds(10);

        digitalWrite(trig, LOW);

        return pulseIn(echo, HIGH, timeoutUs);
    }

};
