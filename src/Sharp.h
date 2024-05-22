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

static const int READING_COUNT = 9;
const uint8_t sharp10_150_distances[READING_COUNT] = {10, 20, 30, 50, 75, 100, 125, 150, 160};


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
    static const int MIN_DISTANCE = 10;
    static const int MAX_DISTANCE = 150;

    explicit Sharp10_150Table(byte pin, const uint16_t *readings1)
            : SharpBase(pin), readings(readings1) {}

    unsigned short getDistance() override {
        unsigned short aRead = getAnalogRead();

        if (aRead > readings[0]) {
            return MIN_DISTANCE;
        }
        if (aRead < readings[READING_COUNT - 1]) {
            return MAX_DISTANCE;
        }

        byte i = 1;
        while (readings[i] > aRead) {
            i++;
        }
        byte min = sharp10_150_distances[i - 1];
        byte max = sharp10_150_distances[i];

        return map(aRead, readings[i - 1], readings[i], min, max);
    }

private:
    const uint16_t *readings;
};




const uint16_t sharp100_550_distances[READING_COUNT] = {60, 80, 100, 200, 300, 400, 500, 550, 600};
class Sharp100_500Table : public SharpBase {

public:
    static const int MIN_DISTANCE = 50;
    static const int MAX_DISTANCE = 550;

    explicit Sharp100_500Table(byte pin, const uint16_t *readings1)
            : SharpBase(pin), readings(readings1) {}

    unsigned short getDistance() override {
        unsigned short aRead = getAnalogRead();

        if (aRead > readings[0]) {
            return MIN_DISTANCE;
        }
        if (aRead < readings[READING_COUNT - 1]) {
            return MAX_DISTANCE;
        }

        byte i = 1;
        while (readings[i] > aRead) {
            i++;
        }
        unsigned short min = sharp100_550_distances[i - 1];
        unsigned short max = sharp100_550_distances[i];

        return map(aRead, readings[i - 1], readings[i], min, max);
    }

private:
    const uint16_t *readings;
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
