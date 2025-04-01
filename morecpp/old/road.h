#pragma once
#include "gameobject.h"
#include "car.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

class Road {
private:
    Vector3 position;
    float length;
    float width;             // Total road width
    int laneCount;
    float laneWidth;         // Width of each lane
    std::vector<std::shared_ptr<Car>> cars;

public:
    Road(const Vector3& pos, float len, float w, int lanes)
        : position(pos), length(len), width(w), laneCount(lanes) {
        laneWidth = width / static_cast<float>(laneCount);
    }

    // Add a car to the road
    void addCar(std::shared_ptr<Car> car);

    // Update all cars on the road
    void update(float deltaTime);

    // Get lane center position (y-coordinate)
    float getLanePosition(int lane) const;

    // Getters
    const Vector3& getPosition() const { return position; }
    float getLength() const { return length; }
    float getWidth() const { return width; }  // Added missing method
    int getLaneCount() const { return laneCount; }
    float getLaneWidth() const { return laneWidth; }
    const std::vector<std::shared_ptr<Car>>& getCars() const { return cars; }
};