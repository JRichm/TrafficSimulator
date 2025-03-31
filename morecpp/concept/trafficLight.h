#pragma once

#include <string>
#include "vec3.h"

enum class TrafficLightState {
	RED,
	YELLOW,
	GREEN,
	LEFT_ARROW, 
	RIGHT_ARROW
};

class TrafficLight {
private:
	std::string id;
	Vector3 position;
	TrafficLightState currentState;
	int fromRoadId;
	int toRoadId;

public:
	TrafficLight(const std::string& id, const Vector3& pos, int fromRoad, int toRoad);

	void setState(TrafficLightState state);
	TrafficLightState getState() const;
	bool allowsMovement() const;
};
