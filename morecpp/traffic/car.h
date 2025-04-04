#pragma once

#include "vehicle.h"

class Car : public Vehicle {
public:
    Car(const Vector3& position, const Vector3& dimensions, const Color& color, float speed)
        : Vehicle(VehicleType::CAR, position, dimensions, color) {
        setPreferredSpeed(speed);
    }
};