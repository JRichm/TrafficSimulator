#pragma once

#include <memory>

#include "trafficLightJunction.h"
#include "simpleJunction.h"

enum class IntersectionType {
	SIMPLE,
	TRAFFIC_LIGHT,
	ROUNDABOUT, 
	T_JUNCTION,
	Y_JUNCTION,
};


class Intersection {
public:
	static std::shared_ptr<Junction> createIntersection(IntersectionType type, const std::string& id, const Vector3& position, float radius = 15.0f) {
		switch (type) {
		case IntersectionType::TRAFFIC_LIGHT:
			return std::make_shared<TrafficLightJunction>(id, position, radius);
		case IntersectionType::SIMPLE:
			return std::make_shared<SimpleJunction>(id, position, radius);
		default:
			return std::make_shared<SimpleJunction>(id, position, radius);
		}
	}
};