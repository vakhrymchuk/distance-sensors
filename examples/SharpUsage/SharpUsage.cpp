#include <Arduino.h>
#include "Interval.h"
#include "Sharp.h"

#define SHARP_PIN A0

//DistanceSensor *sensor1;
Sharp10_150 *sensor1;

Interval showInterval(100);
Interval readInterval(10);

void setup() {
    Serial.begin(115200);
    sensor1 = new Sharp10_150(SHARP_PIN);
}

unsigned short l;

void loop() {
    if (readInterval.isReady()) {
        l = sensor1->getDistance();
    }

    if (showInterval.isReady()) {
        Serial.print("\tleft = ");
        Serial.print(l);
        Serial.println();
    }
}
