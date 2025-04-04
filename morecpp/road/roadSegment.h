#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#include "../core/gameobject.h"
#include "../core/vec3.h"
#include "lane.h"


// forward declaration
class Junction;
class Vehicle;


class RoadSegment : public GameObject, public std::enable_shared_from_this<RoadSegment> {
protected:
	std::string id;
	float length;
	float speedLimit;
	std::vector<Lane> lanes;
	std::weak_ptr<Junction> startJunction;
	std::weak_ptr<Junction> endJunction;

	struct LaneTransition {
		float startDistance;
		float endDistance;
		int startLaneCount;
		int endLaneCount;
		std::unordered_map<int, int> laneMapping;
	};

	std::vector<LaneTransition> laneTransitions;
	std::vector<std::shared_ptr<Vehicle>> vehicles;


public:
	RoadSegment(const std::string& id, const Vector3& pos, const Vector3& dim, float speedLimit);

	void setJunctions(std::shared_ptr<Junction> start, std::shared_ptr<Junction> end);
	void addLane(const Lane& lane);
	void addLaneTransition(float startDist, float endDist, int startLanes, int endLanes, const std::map<int, int>& mapping);

	void addVehicle(std::shared_ptr<Vehicle> vehicle);
	void removeVehicle(std::shared_ptr<Vehicle> vehicle);

	void update(float deltaTime) override;

	// get position and path
	Vector3 getPositionAt(float distance) const;
	Vector3 getDirectionAt(float distance) const;
	Vector3 getLanePositionAt(int laneIndex, float distance) const;

	// get lanes
	const std::vector<Lane>& getLanes() const { return lanes; }
	int getLaneCount() const;
	int getLaneCountAt(float distance) const;
	bool isValidLane(int laneIndex, float distance) const { return laneIndex >= 0 && laneIndex < getLaneCountAt(distance); }
	int getTargetLane(int currentLane, float currentDistance, float lookAheadDistance) const;
	int determineClosestLane(float yPosition) const;

	// get vehicles
	const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const { return vehicles; }
	std::vector<std::shared_ptr<Vehicle>> getVehiclesInLane(int laneIndex) const;
	std::vector<std::shared_ptr<Vehicle>> getVehiclesInLaneSection(int laneIndex, float startDist, float endDist) const;

	const std::string& getId() const { return id; }
	float getLength() const { return length; }
	float getSpeedLimit() const { return speedLimit; }
	std::shared_ptr<Junction> getStartJunction() const { return startJunction.lock(); }
	std::shared_ptr<Junction> getEndJunction() const { return endJunction.lock(); }
};