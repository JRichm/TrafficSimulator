#pragma once

#include "junction.h"


enum LightState {
	GREEN,
	YELLOW,
	RED
};


class TrafficLightJunction : public Junction {
public:
	TrafficLightJunction(const std::string& id, const Vector3& pos, float radius = 15.0f) :
		Junction(id, pos, radius),
		currentGreenLight(0),
		greenDuration(5.0f),
		yellowDuration(2.0f) {}


	void connectRoad(std::shared_ptr<RoadSegment>road) override {
		Junction::connectRoad(road);

		TrafficLight light;
		light.state = connectedRoads.size() == 1 ? LightState::GREEN : LightState::RED;
		light.timer = 0.0f;
		light.roadIndex = connectedRoads.size() - 1;
		
		trafficLights.push_back(light);
	}


	void update(float deltaTime) override {
		if (trafficLights.empty()) return;


		TrafficLight& currentLight = trafficLights[currentGreenLight];
		currentLight.timer += deltaTime;

		switch (currentLight.state) {
			case LightState::GREEN:
				if (currentLight.timer >= greenDuration) {
					currentLight.timer = 0.0f;
					currentLight.state = LightState::YELLOW;
				}
				break;

			case LightState::YELLOW:
				if (currentLight.timer >= yellowDuration) {
					currentLight.timer = 0.0f;
					currentLight.state = LightState::RED;

					currentGreenLight = (currentGreenLight + 1) % trafficLights.size();
					trafficLights[currentGreenLight].state = LightState::GREEN;
					trafficLights[currentGreenLight].timer = 0.0f;
				}
				break;

			default:
				break;
		}
	}


	bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override {
		int roadIndex = getRoadIndex(fromRoad);
		if (roadIndex < 0 || roadIndex >= trafficLights.size()) return false;

		// True on green
		return trafficLights[roadIndex].state = LightState::GREEN;
	}

	
	LightState getLightState(std::shared_ptr<RoadSegment> road) const {
		int roadIndex = getRoadIndex(road);
		if (roadIndex >= 0 && roadIndex < trafficLights.size()) {
			return trafficLights[roadIndex].state;
		}
		return LightState::RED;
	}


	//	TrafficLightJunction(const std::string& id, const Vector3& pos) :
	//		Junction(id, pos), timer(0.0f), greenDuration(5.0f),yellowDuration(2.0f) {}
	//
	//	void update(float deltaTime) override {
	//		timer += deltaTime;
	//		updateLights();
	//	}
	// 
	//	bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override {
	//		int roadIndex = getRoadIndex(fromRoad);
	//		return roadIndex >= 0 && lightStates[roadIndex] == GREEN;
	//	}


private:
	struct TrafficLight {
		LightState state;
		float timer;
		int roadIndex;
	};

	std::vector<TrafficLight> trafficLights;

	int currentGreenLight;
	float greenDuration;
	float yellowDuration;
};