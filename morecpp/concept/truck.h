#pragma once

#include "vehicle.h"

class Truck : public Vehicle {
private:
	float cargoWeight;

public:
	Truck(const Vector3& pos, const Vector3& dim, const Color& col, float preferredSpeed = 4.0f);

	void adjustSpeedForTraffic(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars, float deltaTime) override;
	bool shouldChangeLane(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars) override;

	void setCargoWeight(float weight);
	float getCargoWeight() const;
};