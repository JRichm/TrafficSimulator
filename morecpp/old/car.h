#pragma once
#include "gameobject.h"
#include <vector>
#include <memory>

// Forward declaration
class Road;

class Car : public GameObject {
private:
    float maxSpeed;
    float currentSpeed;
    float preferredSpeed;
    Vector3 velocity;
    float laneChangeTimer;
    float minLaneChangeTime;
    char carIcon;
    Road* road;

public:
    Car(const Vector3& pos, const Vector3& dim, const Color& col,
        float preferredSpeed = 5.0f, float maxSpeedLimit = 10.0f, char icon = '>')
        : GameObject(pos, dim, col),
        maxSpeed(maxSpeedLimit),
        currentSpeed(preferredSpeed),
        preferredSpeed(preferredSpeed),
        laneChangeTimer(0.0f),
        minLaneChangeTime(3.0f),
        carIcon(icon),
        road(nullptr) {}

    void setRoad(Road* r);
    void update(float deltaTime) override;
    int getCurrentLane() const;
    void changeLane(int direction);
    void adjustSpeedForTraffic(const std::vector<std::shared_ptr<Car>>& nearbyCars, float deltaTime);
    bool shouldChangeLane(const std::vector<std::shared_ptr<Car>>& nearbyCars);

    // Getters
    float getCurrentSpeed() const { return currentSpeed; }
    float getPreferredSpeed() const { return preferredSpeed; }
    float getMaxSpeed() const { return maxSpeed; }
    char getIcon() const { return carIcon; }
    float getLaneChangeTimer() const { return laneChangeTimer; }
    float getMinLaneChangeTime() const { return minLaneChangeTime; }

    // Setters
    void setCurrentSpeed(float speed) { currentSpeed = std::min(speed, maxSpeed); }
    void setPreferredSpeed(float speed) { preferredSpeed = std::min(speed, maxSpeed); }
    void resetLaneChangeTimer() { laneChangeTimer = 0.0f; }
};