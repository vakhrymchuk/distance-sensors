#include <Arduino.h>
#include <Wire.h>
#include "i2cScanner.h"
#include "Interval.h"
#include "Vl53l0xSensorPin.h"
#include "Vl53l1xSensorPin.h"

DistanceSensor *sensor1;
DistanceSensor *sensor2;

Interval showInterval(100);
Interval readInterval(100);


void setupVl53() {
    checkWire(Wire);

    sensor1 = new Vl53l0xSensorPin(5);
    sensor2 = new Vl53l1xSensorPin(6);

    sensor1->initSensor();
    sensor2->initSensor();

}

void setup() {

    Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);

    setupVl53();

}

unsigned short l;
unsigned short r;

void loop() {

    if (readInterval.isReady()) {
        l = sensor1->getDistance();
        r = sensor2->getDistance();
    }

    if (showInterval.isReady()) {

        Serial.print("\tleft = ");
        Serial.print(l);
        Serial.print("\tright = ");
        Serial.print(r);

        Serial.println();

    }
}
