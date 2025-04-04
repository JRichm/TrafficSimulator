#pragma once

#include <vector>
#include <memory>

#include "../core/gameobject.h"
#include "../core/vec3.h"
#include "../road/junction.h"
#include "../road/roadSegment.h"
#include "../road/highwayRamp.h"


enum class VehicleType {
	CAR,
	TRUCK,
	BUS,
	MOTORCYCLE
};


enum class VehicleState {
	CRUISING,
	FOLLOWING,
	LANE_CHANGING,
	MERGING,
	TURNING,
	STOPPED
};


// forward declaration
class Destination;


class Vehicle : public GameObject {
protected:
	VehicleType type;
	VehicleState state;
	Vector3 velocity;

	std::shared_ptr<RoadSegment> currentRoad;
	float distanceAlongRoad;
	int currentLane;

	std::shared_ptr<Destination> destination;
	std::vector<std::shared_ptr<RoadSegment>> plannedRoute;

	float maxSpeed;
	float preferredSpeed;
	float currentSpeed;

	float laneChangeTimer;
	float minLaneChangeTime;


public:
	Vehicle(VehicleType type, const Vector3& pos, const Vector3& dim, const Color& col);

	void update(float deltaTime) override;

	void setCurrentRoad(std::shared_ptr<RoadSegment> road, float distance, int lane);
	void setDestination(std::shared_ptr<Destination> dest);
	void planRoute();

	virtual void adjustSpeedForTraffic(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars, float deltaTime);
	virtual bool shouldChangeLane(const std::vector<std::shared_ptr<Vehicle>>& nearbyCars);
	virtual void changeLane(int direction);
	virtual void handleIntersection(std::shared_ptr<Junction> junction);
	virtual void handleRamp(HighwayRamp* ramp, float deltaTime);


	VehicleType getType() const { return type; }
	VehicleState getState() const { return state; }
	Color getColor() const { return color; }
	const Vector3& getVelocity() const { return velocity; }
	std::shared_ptr<RoadSegment> getCurrentRoad() const { return currentRoad; }
	float getDistanceAlongRoad() const { return distanceAlongRoad; }
	int getCurrentLane() const { return currentLane; }
	std::shared_ptr<Destination> getDestination() const { return destination; }
	float getCurrentSpeed() const { return currentSpeed; }
	float getPreferredSpeed() const { return preferredSpeed; }

	void setState(VehicleState newState) { state = newState; }
	void setVelocity(const Vector3& vel) { velocity = vel; }
	void setCurrentSpeed(float speed) { currentSpeed = std::max(0.0f, std::min(speed, maxSpeed)); }
	void setPreferredSpeed(float speed) { preferredSpeed = std::max(0.0f, std::min(speed, maxSpeed)); }
};
