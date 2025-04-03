#include <random>


#include "vehicle.h"

Vehicle::Vehicle(VehicleType type, const Vector3& pos, const Vector3& dim, const Color& col)
  : GameObject(pos, dim, col),
	type(type),
	state(VehicleState::CRUISING),
	velocity(0, 0, 0),
	currentRoad(nullptr),
	distanceAlongRoad(0.0f),
	currentLane(0),
	destination(nullptr),
	maxSpeed(10.0f),
	preferredSpeed(5.0f),
	currentSpeed(0.0f),
	laneChangeTimer(3.0f) {
}

void Vehicle::update(float deltaTime) {
	laneChangeTimer += deltaTime;

	if (!currentRoad) {
		return;
	}


	// get nearby cars
	std::vector<std::shared_ptr<Vehicle>> nearbyCars = currentRoad->getVehiclesInLane(currentLane);


	// slow down if cars ahead
	adjustSpeedForTraffic(nearbyCars, deltaTime);


	// check if we need to change lane
	if (state == VehicleState::CRUISING && laneChangeTimer > minLaneChangeTime) {
		if (shouldChangeLane(nearbyCars)) {

			// check wich lane to change to
			int laneCount = currentRoad->getLaneCount();
			int direction = 0;


			// if in left most lane
			if (currentLane == 0) {
				direction = 1;
			}

			// if in right most lane
			else if (currentLane == laneCount - 1) {
				direction = -1;
			}

			// pick random lane 
			else {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> distrib(0, 1);
				direction = distrib(gen) ? 1 : -1;
			}

			changeLane(direction);
			laneChangeTimer = 0.0f;
		}
	}

	// check if on ramp
	auto ramp = dynamic_cast<HighwayRamp*>(currentRoad.get());
	if (ramp) {
		handleRamp(ramp, deltaTime);
	}


	// check for lane transitions
	float lookAheadDistance = 30;
	int targetLane = currentRoad->getTargetLane(currentLane, distanceAlongRoad, lookAheadDistance);

	if (targetLane != currentLane && laneChangeTimer > minLaneChangeTime / 2) {
		int direction = targetLane > currentLane ? 1 : -1;
		changeLane(direction);
		laneChangeTimer = 0.0f;
	}

	// update velocity and position
	Vector3 roadDirection = currentRoad->getDirectionAt(distanceAlongRoad);
	velocity = roadDirection * currentSpeed;

	distanceAlongRoad += currentSpeed * deltaTime;

	position = currentRoad->getLanePositionAt(currentLane, distanceAlongRoad);
}