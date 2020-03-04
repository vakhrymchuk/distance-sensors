#pragma once

#include "DistanceSensor.h"
#include "ADC.h"


class SharpBase : public DistanceSensor {

public:
    static constexpr float VOLTAGE_TO_ADC_FACTOR = 5.0 / 1024;

    SharpBase(byte pin) : pin(pin) {
//        pinMode(pin, INPUT);
    }

    unsigned short getAnalogRead() final {
        return analogRead(pin);
    }

    float getVoltage() {
        return filter(analogRead(pin)) * VOLTAGE_TO_ADC_FACTOR;
    }

protected:

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


const byte sharp10_150_distances[] = {7, 10, 15, 22, 32, 47, 70, 104, 130};
static const int SENSORS_COUNT = 5;
static const int READING_COUNT = 9;
unsigned short sharp10_150[SENSORS_COUNT][READING_COUNT] = {
        {860, 760, 540, 395, 295, 219, 155, 118, 90},
        {840, 750, 530, 392, 295, 233, 185, 153, 137},
        {867, 769, 550, 400, 300, 230, 172, 145, 120},
        {870, 780, 526, 370, 265, 195, 157, 125, 103},
        {818, 758, 565, 420, 327, 263, 215, 195, 160}
};


class Sharp10_150 : public SharpBase {

public:
    static const int MIN_DISTANCE = 6;
    static const int MAX_DISTANCE = 150;

    explicit Sharp10_150(byte pin) : SharpBase(pin) {}


    unsigned short getDistance() final {
        float v = getVoltage();
//        unsigned short dist = (unsigned short) (57.0284 * pow(v, -1.4522)); // калибровка
        auto dist = (unsigned short) (60.101 * pow(v, -1.535)); // по графику
        return constrain(dist, MIN_DISTANCE, MAX_DISTANCE);
    }

};


class Sharp10_150Table : public SharpBase {

public:
    static const int MIN_DISTANCE = 6;
    static const int MAX_DISTANCE = 150;

    explicit Sharp10_150Table(byte pin) : SharpBase(pin), sensorIndex(pin - A0) {}

    unsigned short getDistance() override {
        unsigned short aRead = getAnalogRead();

        auto sensorsReadings = sharp10_150[sensorIndex];

        if (aRead > sensorsReadings[0]) {
            return MIN_DISTANCE;
        }
        if (aRead < sensorsReadings[READING_COUNT - 1]) {
            return MIN_DISTANCE;
        }

        byte i = 1;
        while (sensorsReadings[i] > aRead) {
            i++;
        }
        byte min = sharp10_150_distances[i - 1];
        byte max = sharp10_150_distances[i];

        return map(aRead, sensorsReadings[i - 1], sensorsReadings[i], min, max);
    }

private:
    const byte sensorIndex;
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
