#pragma once

#include "DistanceSensor.h"

class MedianFilterWindow : public DistanceSensor {
public:

    static const int ARR_SIZE = 3;
    static const int MEDIAN_INDEX = ARR_SIZE >> 1;

    MedianFilterWindow(DistanceSensor *sensor, int arrSize = ARR_SIZE) : sensor(sensor) {
        values = new unsigned short[ARR_SIZE];
        for (int i = 0; i < ARR_SIZE; ++i) values[i] = 0;
        valuesSorted = new unsigned short[ARR_SIZE];
        for (int i = 0; i < ARR_SIZE; ++i) valuesSorted[i] = 0;
    }

    unsigned short getDistance() override final {
        pushNewValue();
        return getMedianValue();
    }

private:

    DistanceSensor *sensor;

    unsigned short *values;
    unsigned short *valuesSorted;

    void pushNewValue();

    void sortValues();

    unsigned short getMedianValue() {
        for (int i = 0; i < ARR_SIZE; ++i) {
            valuesSorted[i] = values[i];
        }
        sortValues();
        return valuesSorted[MEDIAN_INDEX];
    }
};

void MedianFilterWindow::pushNewValue() {
    for (int i = 1; i < ARR_SIZE; ++i) {
        values[i] = values[i - 1];
    }
    values[0] = sensor->getDistance();
}

void MedianFilterWindow::sortValues() {
    int minIndex;
    for (int position = 0; position <= MEDIAN_INDEX; ++position) {
        minIndex = position;

        for (int j = position + 1; j < ARR_SIZE; ++j) {
            if (valuesSorted[j] < valuesSorted[minIndex]) {
                minIndex = j;
            }
        }
        // swap value
        if (position != minIndex) {
            unsigned short temp = valuesSorted[position];
            valuesSorted[position] = valuesSorted[minIndex];
            valuesSorted[minIndex] = temp;
        }
    }
}
