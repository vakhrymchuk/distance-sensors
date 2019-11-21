#include <Arduino.h>
#include <Interval.h>
#include <Wire.h>
#include "Vl53l0xSensor.h"

DistanceSensor *sensor1;
DistanceSensor *sensor2;

Interval showInterval(100);
Interval readInterval(100);

byte checkWire(TwoWire wire) {
    byte working = 0;
    byte count = 0;
    wire.begin();
    for (byte i = 8; i < 120; i++) {
        wire.beginTransmission(i);
        if (wire.endTransmission() == 0) {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
            delay(10);
            working = i;
        }
    }
    Serial.println("Done.");
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");
//    wire.end();
    return working;
}


void setupVl53() {
    checkWire(Wire);

    Vl53l0xSensor::lowPin(5);
    Vl53l0xSensor::lowPin(6);

    Vl53l0xSensor *sensor11 = new Vl53l0xSensor(5);
    sensor1 = sensor11;
    Vl53l0xSensor *sensor22 = new Vl53l0xSensor(6);
    sensor2 = sensor22;

}

void setup() {

    Serial.begin(115200);

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
