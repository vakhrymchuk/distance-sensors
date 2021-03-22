#include <Arduino.h>
#include <Wire.h>
#include "i2cScanner.h"
#include "Interval.h"
#include "Vl53l0xSensor.h"
#include "TimingSensor.h"

TimingSensor *timingSensor;

Interval showInterval(100);
Interval readInterval(100);


void setupVl53() {
    checkWire(Wire);

    auto *vl53l0x = new Vl53l0xSensor();
    vl53l0x->initSensor();
    timingSensor = new TimingSensor(vl53l0x);

}

void setup() {

    Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);

    setupVl53();

}

unsigned short distance;

void loop() {

    if (readInterval.isReady()) {
        distance = timingSensor->getDistance();
    }

    if (showInterval.isReady()) {
        Serial.print("\tdistance = ");
        Serial.print(distance);
        Serial.print("\ttime = ");
        Serial.print(timingSensor->time());
        if (timingSensor->isLongerThan(4000)) Serial.print("\tLONG!");
        Serial.println();
    }
}
