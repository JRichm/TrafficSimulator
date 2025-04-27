#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

#include "vehicle.h"
#include "../navigation/routeManager.h"


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


void Vehicle::setCurrentRoad(std::shared_ptr<RoadSegment> road, float distance, int lane) {
	currentRoad = road;
	distanceAlongRoad = distance;
	currentLane = lane;

	// update position
	if (road) {
		position = road->getLanePositionAt(lane, distance);
	}
}


void Vehicle::setDestination(std::shared_ptr<Destination> dest) {
	destination = dest;

	if (dest && currentRoad) {
		std::cout << "need plan_route() function" << std::endl;
	}
}


void Vehicle::adjustSpeedForTraffic(const std::vector <std::shared_ptr<Vehicle>>& nearbyCars, float deltaTime) {
	float targetSpeed = preferredSpeed;
	float minDistance = 1000.0f;
	float safeDistance = 5.0f;
	float followingDistance = 10.0f;

	float vehicleLength = dimensions.x;

	for (const auto& otherCar : nearbyCars) {
		if (otherCar.get() == this) continue;

		// check if car is ahead
		float otherDistance = otherCar->getDistanceAlongRoad();
		float distance = otherDistance - distanceAlongRoad;
		
		if (distance > 0 && distance < minDistance) {
			minDistance = distance;

			float actualGap = distance - (vehicleLength + otherCar->getDimensions().x) / 2;

			if (actualGap < safeDistance * 0.5f) {
				targetSpeed = 0.0f;
			}

			else if (actualGap < safeDistance) {
				targetSpeed = otherCar->getCurrentSpeed() * 0.5f;
			}

			else if (actualGap < followingDistance) {
				targetSpeed = otherCar->getCurrentSpeed() * 0.8f;
			}

			else if (actualGap < followingDistance * 2) {
				targetSpeed = otherCar->getCurrentSpeed() * 0.9f;
			}
		}
	}

	// check speed limit
	targetSpeed = std::min(targetSpeed, currentRoad->getSpeedLimit());

	float accelerationRate = 2.0f;
	float normalDecelerationRate = 4.0f;
	float emergencyDecelerationRate = 8.0f;

	// acceleration
	if (currentSpeed < targetSpeed) {
		currentSpeed += accelerationRate * deltaTime;
		if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
	}

	else if (currentSpeed > targetSpeed && targetSpeed < preferredSpeed * 0.6f) {
		currentSpeed -= emergencyDecelerationRate * deltaTime;
		if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
	}

	else if (currentSpeed > targetSpeed) {
		currentSpeed -= normalDecelerationRate * deltaTime;
		if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
	}

	currentSpeed = std::max(0.0f, std::min(currentSpeed, maxSpeed));
}


bool Vehicle::shouldChangeLane(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars) {
	bool carAheadTooClose = false;
	float minDistanceAhead = 1000.0f;

	for (const auto& otherCar : nearbyCars) {
		if (otherCar.get() == this) continue;

		float otherDistance = otherCar->getDistanceAlongRoad();
		float distance = otherDistance - distanceAlongRoad;

		if (distance > 0 && distance < minDistanceAhead) {
			minDistanceAhead = distance;

			if (distance < 8.0f && otherCar->getCurrentSpeed() < preferredSpeed * 0.9f) {
				carAheadTooClose = true;
			}
		}
	}

	return carAheadTooClose;
}


void Vehicle::changeLane(int direction) {
	if (!currentRoad) return;

	int targetLane = currentLane + direction;

	if (currentRoad->isValidLane(targetLane, distanceAlongRoad)) {
		currentLane = targetLane;
		state = VehicleState::LANE_CHANGING;
		laneChangeTimer = 0.0f;
	}
}


void Vehicle::handleIntersection(std::shared_ptr<Junction> junction) {
	if (!junction || plannedRoute.empty()) {
		return;
	}

	std::shared_ptr<RoadSegment> nextRoad = nullptr;

	if (currentRoad->getId() == plannedRoute.back()->getId()) {
		return;
	}

	auto it = std::find_if(plannedRoute.begin(), plannedRoute.end(), [this](const std::shared_ptr<RoadSegment>& road) {
		return road->getId() == currentRoad->getId();
		});
	 
	if (it != plannedRoute.end() && it + 1 != plannedRoute.end()) {
		nextRoad = *(it + 1);
	}

	if (!nextRoad) {
		return;
	}

	// try to navigate to the next road
	if (junction->canNavigate(currentRoad, nextRoad, this)) {

		// pick random lane on the next road
		int nextLane = 0;
		if (nextRoad->getLaneCount() > 1) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distrib(1, nextRoad->getLaneCount() - 2);
			nextLane = distrib(gen);
		}

		setCurrentRoad(nextRoad, 0.0f, nextLane);

		state = VehicleState::TURNING;

	// cant navigate
	} else {
		currentSpeed = 0.0f;
		state = VehicleState::STOPPED;
	}
}


void Vehicle::handleRamp(HighwayRamp* ramp, float deltaTime) {
	if (!ramp) return;

	auto mainRoad = ramp->getMainRoad().lock();
	if (!mainRoad) return;

	MergeInfo mergeInfo = ramp->getMergeParameters();

	if (ramp->getType() == RampType::ENTRANCE) {
		float mergePoint = (mergeInfo.mergeStartDistance + mergeInfo.mergeEndDistance) / 2;

		float rampProgress = distanceAlongRoad / ramp->getLength();

		if (rampProgress > 0.75f) {
			std::vector<std::shared_ptr<Vehicle>> targetLaneVehicles = mainRoad->getVehiclesInLaneSection(
				mergeInfo.targetLane,
				mergeInfo.mergeStartDistance,
				mergeInfo.mergeEndDistance
			);

			bool canMerge = true;
			for (const auto& otherVehicle : targetLaneVehicles) {
				float distance = std::abs(otherVehicle->getDistanceAlongRoad() - mergePoint);
				if (distance < 10.0f) {
					canMerge = false;
					break;
				}
			}

			if (canMerge && rampProgress > 0.9f) {
				setCurrentRoad(mainRoad, mergePoint, mergeInfo.targetLane);
				currentSpeed = std::min(mainRoad->getSpeedLimit(), maxSpeed);
				state = VehicleState::MERGING;
			} else if (rampProgress > 0.95f) {
				currentSpeed = currentSpeed * 0.8f;
			} else {
				float targetSpeed = std::min(mainRoad->getSpeedLimit(), maxSpeed);
				if (currentSpeed < targetSpeed) {
					currentSpeed += 3.0f * deltaTime;
				}
			}
		}
	} else if (ramp->getType() == RampType::EXIT) {
		float rampProgress = distanceAlongRoad / ramp->getLength();
		if (rampProgress < 0.3f) {
			float targetSpeed = ramp->getSpeedLimit();
			if (currentSpeed > targetSpeed) {
				currentSpeed -= 3.0f * deltaTime;
			}
		}
	}
}
