#ifndef SHARP_H
#define SHARP_H

#include "DistanceSensor.h"
#include "ADC.h"


class SharpBase : public DistanceSensor {

public:
    static constexpr float VOLTAGE_TO_ADC_FACTOR = 5.0 / 1024;

    SharpBase(byte pin) : pin(pin) {
//        pinMode(pin, INPUT);
    }

protected:
    float getVoltage() {
        return filter(analogRead(pin)) * VOLTAGE_TO_ADC_FACTOR;
    }

    byte get128value() const {
        return (byte) (analogRead(pin) >> 3);
    }

private:
    const byte pin;


// переменные для калмана
    float varVolt = 221;
    float varProcess = 15; // скорость реакции на изменение (подбирается вручную)
    float Pc = 0.0;
    float G = 0.0;
    float P = 1.0;
    float Xp = 0.0;
    float Zp = 0.0;
    float Xe = 0.0;
// переменные для калмана

    float filter(float val) {
        Pc = P + varProcess;
        G = Pc / (Pc + varVolt);
        P = (1 - G) * Pc;
        Xp = Xe;
        Zp = Xp;
        Xe = G * (val - Zp) + Xp; // "фильтрованное" значение
        return (Xe);
    }

};

class Sharp : public SharpBase {

public:
    static const int MIN_DISTANCE = 18;
    static const int MAX_DISTANCE = 150;

    Sharp(byte pin) : SharpBase(pin) {}

    unsigned short getDistance() override final {
        float v = getVoltage();
        if (v < 0.46 || v > 2.8) {
            return MAX_DISTANCE;
        }
        return (unsigned short) (60.668 * pow(v, -1.167));
//        return (unsigned short) constrain(dist, MIN_DISTANCE, MAX_DISTANCE);
    }
};


const byte sharp20_150_table128[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 192, 170, 152, 137, 124, 113, 103, 95, 88, 81, 76, 71, 66, 62, 59, 55, 52,
        50, 47, 45, 43, 41, 39, 37, 36, 34, 33, 31, 30, 29, 28, 27, 26, 25, 24, 24, 23, 22, 22, 21, 20, 20, 19, 19, 18,
        18, 17, 17, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10, 10,
        10, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        5, 5, 5, 5, 5, 5, 5
};

class Sharp10_150 : public SharpBase {

public:
    static const int MIN_DISTANCE = 6;
    static const int MAX_DISTANCE = 150;

    Sharp10_150(byte pin) : SharpBase(pin) {}


    unsigned short getDistance() override final {
        float v = getVoltage();
//        Serial.println(v);
//        if (v < 0.46 || v > 2.8) {
//            return MAX_DISTANCE;
//        }
//        unsigned short dist = (unsigned short) (57.0284 * pow(v, -1.4522)); // калибровка
        unsigned short dist = (unsigned short) (60.101 * pow(v, -1.535)); // по графику
        return constrain(dist, MIN_DISTANCE, MAX_DISTANCE);
    }


//    virtual unsigned short getDistance() {
//        return sharp20_150_table128[get128value()];
//    }
};


class SharpMini : public SharpBase {

public:
    static const int MIN_DISTANCE = 6;
    static const int MAX_DISTANCE = 80;

    SharpMini(byte pin) : SharpBase(pin) {}

    unsigned short getDistance() override final {
        float v = getVoltage();
        if (v < 0.42 || v > 3.3) {
            return MAX_DISTANCE;
        }
        unsigned short dist = (unsigned short) (26.974 * pow(v, -1.249));
        return constrain(dist, MIN_DISTANCE, MAX_DISTANCE);
    }
};

class SharpMicro : public SharpBase {

public:
    static const int MIN_DISTANCE = 1;
    static const int MAX_DISTANCE = 20;

    SharpMicro(byte pin) : SharpBase(pin) {}

    unsigned short getDistance() override final {
        float v = getVoltage();
        if (v < 0.32 || v > 2.5) {
            return MAX_DISTANCE;
        }
        return (unsigned short) (4.9115 * pow(v, -1.2548));
//        return constrain(dist, MIN_DISTANCE, MAX_DISTANCE);
    }
};

#endif
