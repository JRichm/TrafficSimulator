#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "../core/vec3.h"


// forward declaration
class RoadSegment;
class Vehicle;


class Junction {
protected:
	std::string id;
	Vector3 position;
	std::vector<std::weak_ptr<RoadSegment>> connectedRoads;
	float radius;
	std::unordered_map<std::string, float> roadAngles;

public:
	Junction(const std::string& id, const Vector3& pos, float radius = 10.0f) :
		id(id), position(pos) , radius(radius) {}

	// connect road
	virtual void connectRoad(std::shared_ptr<RoadSegment> road);


	// getters
	int getRoadIndex(std::shared_ptr<RoadSegment> road) const;
	float getRadius() const { return radius; }
	const std::string& getId() const { return id; }
	const Vector3& getPosition() const { return position; }
	std::vector<std::shared_ptr<RoadSegment>> getConnectedRoads() const;
	float getAngleBetweenRoads(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad) const;


	// virtual declarations
	virtual ~Junction() = default;
	virtual std::vector<std::shared_ptr<RoadSegment>> getExitRoads(std::shared_ptr<RoadSegment> entryRoad) const;
	virtual bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) = 0;
	virtual void update(float deltaTime) {}
};