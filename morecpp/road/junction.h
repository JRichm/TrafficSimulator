#pragma once

#include <vector>
#include <memory>
#include <string>
#include "../core/vec3.h"


// forward declaration
class RoadSegment;
class Vehicle;


class Junction {
protected:
	std::string id;
	Vector3 position;
	std::vector<std::weak_ptr<RoadSegment>> connectedRoads;


public:
	Junction(const std::string& id, const Vector3& pos) : id(id), position(pos) {}
	virtual ~Junction() = default;

	void connectRoad(std::shared_ptr<RoadSegment> road) {
		connectedRoads.push_back(road);
	}

	std::vector<std::shared_ptr<RoadSegment>> getConnectedRoads() const {
		std::vector<std::shared_ptr<RoadSegment>> roads;
		for (const auto& weakRoad : connectedRoads) {
			if (auto road = weakRoad.lock()) {
				roads.push_back(road);
			}
		}
		return roads;
	}

	virtual std::vector<std::shared_ptr<RoadSegment>> getExitRoads(std::shared_ptr<RoadSegment> entryRoad) const {
		std::vector<std::shared_ptr<RoadSegment>> exits;
		for (const auto& weakRoad : connectedRoads) {
			if (auto road = weakRoad.lock()) {
				if (road != entryRoad) {
					exits.push_back(road);
				}
			}
		}
		return exits;
	}

	virtual bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) = 0;


	virtual void update(float deltaTime) {}

	const std::string& getId() const { return id; }
	const Vector3& getPosition() const { return position; }
};