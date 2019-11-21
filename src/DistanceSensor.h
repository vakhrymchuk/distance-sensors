#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
public:
    virtual unsigned short getDistance() = 0;
};

class DistanceSensorValue : public DistanceSensor {
public:
    unsigned short getDistance() override {
        return value;
    };

    unsigned short value = 0;
};

#endif
