#pragma once

#include "junction.h"

class TrafficLightJunction : public Junction {
public:
	enum LightState {
		GREEN,
		YELLOW,
		RED
	};
	std::vector<LightState> lightState;
	float timer;
	float greenDuration;
	float yellowDuration;


	TrafficLightJunction(const std::string& id, const Vector3& pos) :
		Junction(id, pos), timer(0.0f), greenDuration(5.0f),yellowDuration(2.0f) {}

	void update(float deltaTime) override {
		timer += deltaTime;
		updateLights();
	}

	bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override {
		int roadIndex = getRoadIndex(fromRoad);
	}


private:
	void updateLights() {}

};