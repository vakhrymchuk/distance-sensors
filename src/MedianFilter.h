#pragma once

#include "DistanceSensor.h"

class MedianFilter : public DistanceSensor {
public:

    static const int ARR_SIZE = 5;
    static const int MEDIAN_INDEX = ARR_SIZE >> 1;
    static const int DELAY_BEFORE_READ_MCS = 200;

    MedianFilter(DistanceSensor *sensor, int arrSize = ARR_SIZE) : sensor(sensor) {
        values = new unsigned short[ARR_SIZE];
        for (int i = 0; i < ARR_SIZE; ++i) values[i] = 0;
    }

    unsigned short getDistance() override final {
//        return sensor->getDistance();
        readValues();
        sortValues();
        return getMedian();
    }

private:

    DistanceSensor *sensor;

    unsigned short *values;

    void readValues();

    void sortValues();

    unsigned short getMedian() const {
        return values[MEDIAN_INDEX];
    }
};

// private methods

void MedianFilter::readValues() {
    sensor->getDistance();
    delayMicroseconds(DELAY_BEFORE_READ_MCS);
    for (int i = 0; i < ARR_SIZE; ++i) {
        values[i] = sensor->getDistance();
    }
}

void MedianFilter::sortValues() {
    int minIndex;
    for (int position = 0; position <= MEDIAN_INDEX; ++position) {
        minIndex = position;

        for (int j = position + 1; j < ARR_SIZE; ++j) {
            if (values[j] < values[minIndex]) {
                minIndex = j;
            }
        }
        // swap values
        if (position != minIndex) {
            unsigned short temp = values[position];
            values[position] = values[minIndex];
            values[minIndex] = temp;
        }
    }
}
