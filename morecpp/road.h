#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "gameobject.h"
#include "car.h" // Now we can include car.h since it only has forward declaration of Road

class Road : public GameObject {
private:
    float length;
    int laneCount;
    float laneWidth;
    std::vector<std::shared_ptr<Car>> cars;

public:
    Road(const Vector3& pos, float roadLength, float width, int numLanes)
        : GameObject(pos, Vector3(roadLength, width, 0), Color(0.5f, 0.5f, 0.5f)),
        length(roadLength),
        laneCount(numLanes),
        laneWidth(width / numLanes) {}

    void addCar(std::shared_ptr<Car> car);
    void update(float deltaTime) override;
    float getLanePosition(int lane) const;
    int getLaneCount() const { return laneCount; }
    float getLaneWidth() const { return laneWidth; }
    const std::vector<std::shared_ptr<Car>>& getCars() const { return cars; }
    float getLength() const { return length; }
};