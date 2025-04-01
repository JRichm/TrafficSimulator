#pragma once

#include "vehicle.h"
#include "highwayRamp.h"

class Car : public Vehicle {
private:
	bool isAutonomous;
	

public:
	Car(const Vector3& pos, const Vector3& dim, const Color& col, float preferredSpeed = 5.0f);

	void adjustSpeedForTraffic(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars, float deltaTime) override;
	bool shouldChangeLane(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars) override;
	void handleRamp(HighwayRamp* ramp, float deltaTime) override;

	void setAutonomous(bool autonomous);
	bool isAutonomousDriving() const; // not sure about this one 
};