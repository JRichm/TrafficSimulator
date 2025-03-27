#pragma once
#include <vector>
#include "gameobject.h"
#include "car.h"

class Road : public GameObject {
private:
    std::vector<Car> cars;
    float length;
    int lanes;

public:
    Road(const Vector3& pos, float roadLength, float width, int numLanes)
        : GameObject(pos, Vector3(roadLength, width, 0), Color(0.5f, 0.5f, 0.5f)),
        length(roadLength), lanes(numLanes) {}

    void addCar(const Car& car) {
        cars.push_back(car);
    }

    void update(float deltaTime) {
        for (auto& car : cars) {
            car.update(deltaTime);
        }
    }

    const std::vector<Car>& getCars() const { return cars; }
    float getLength() const { return length; }
    int getLanes() const { return lanes; }
};