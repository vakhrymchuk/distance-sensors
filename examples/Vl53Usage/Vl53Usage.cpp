#include <Arduino.h>
#include <Interval.h>
#include <Wire.h>
#include "Vl53l0xSensorPin.h"
#include "Vl53l1xSensorPin.h"

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

    sensor1 = new Vl53l0xSensorPin(5);
    sensor2 = new Vl53l1xSensorPin(6);

    ((Vl53l0xSensorPin*) sensor1)->initSensor();
    ((Vl53l1xSensorPin*) sensor2)->initSensor();

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
