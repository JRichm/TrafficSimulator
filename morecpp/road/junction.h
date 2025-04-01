#pragma once

#include <vector>
#include <memory>
#include <string>
#include "vec3.h"


// forward declaration
class RoadSegment;
class Vehicle;


class Junction {
protected:
	std::string id;
	Vector3 position;
	std::vector<std::weak_ptr<RoadSegment>> connectedRoads;


public:
	Junction(const std::string& id, const Vector3& pos);
	virtual ~Junction() = default;

	void connectRoad(std::shared_ptr<RoadSegment> road);
	std::vector<std::shared_ptr<RoadSegment>> getConnectedRoads() const;

	virtual std::vector<std::shared_ptr<RoadSegment>> getExitRoads(std::shared_ptr<RoadSegment> entryRoad) const;
	virtual bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) = 0;

	virtual void update(float deltaTime);

	const std::string& getId() const;
	const Vector3& getPosition() const;
};