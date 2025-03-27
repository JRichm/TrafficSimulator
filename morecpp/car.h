#pragma once
#include "gameobject.h"

class Car : public GameObject {
private:
    float speed;
    Vector3 velocity;

public:
    Car(const Vector3& pos, const Vector3& dim, const Color& col, float initialSpeed = 5.0f)
        : GameObject(pos, dim, col), speed(initialSpeed) {}

    void update(float deltaTime) override {
        // Move car along x-axis
        velocity = Vector3(speed, 0, 0);
        position = position + (velocity * deltaTime);
        updateBounds();
    }

    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getSpeed() const { return speed; }
};