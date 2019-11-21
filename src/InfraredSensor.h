#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

class InfraredSensor {
public:
    InfraredSensor(byte pin) : _pin(pin) {
        pinMode(_pin, INPUT);
    }

    byte getColor() {
        return (byte) analogRead(_pin);
    }

private:
    const byte _pin;

};

#endif
