#pragma once

#include <vector>
#include <memory>

#include "../core/gameobject.h"
#include "../core/vec3.h"
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

	// getters
	VehicleType getType() const;
	VehicleState getState() const;
	const Vector3& getVelocity() const;
	std::shared_ptr<RoadSegment> getCurrentRoad() const;
	float getDistanceAlongRoad() const;
	int getCurrentLane() const;
	std::shared_ptr<Destination> getDestination() const;
	float getCurrentSpeed() const;
	float getPreferredSpeed() const;

	// setters
	void setState(VehicleState newState);
	void setVelocity(const Vector3& vel);
	void setCurrentSpeed(float speed);
	void setPreferredSpeed(float speed);
};