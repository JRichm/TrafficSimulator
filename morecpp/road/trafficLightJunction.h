#pragma once

#include "junction.h"
#include "roadSegment.h"

enum LightState {
	GREEN,
	YELLOW,
	RED
};


class TrafficLightJunction : public Junction {
public:
	TrafficLightJunction(const std::string& id, const Vector3& pos, float radius = 15.0f) :
		Junction(id, pos, radius),
		currentPhase(0),
		phaseTimer(0.0f),
		currentGreenLight(0),
		greenDuration(5.0f),
		yellowDuration(2.0f) {}


	void connectRoad(std::shared_ptr<RoadSegment>road) override {
		Junction::connectRoad(road);

		TrafficLight light;
		light.state = connectedRoads.size() == 1 ? LightState::GREEN : LightState::RED;
		light.timer = 0.0f;
		light.roadIndex = static_cast<int>(connectedRoads.size() - 1);

		trafficLights.push_back(light);
	}


	void update(float deltaTime) override {
		phaseTimer += deltaTime;

		if (!phases.empty() && phaseTimer >= phases[currentPhase].duration) {
			phaseTimer = 0.0f;
			currentPhase = (currentPhase + 1) % phases.size();
		}
	}


	bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override {
		if (phases.empty()) return true;

		for (const auto& movement : phases[currentPhase].allowedMovements) {
			if (movement.first == fromRoad->getId() && movement.second == toRoad->getId()) {
				return true;
			}
		}
		return false;
	}


	void generatePhases() {
		auto connectedRoadsCopy = getConnectedRoads();
		if (connectedRoadsCopy.empty()) {
			return;
		}

		std::vector<std::vector<std::shared_ptr<RoadSegment>>> phaseGroups;

		// add phase to each road
		for (const auto& fromRoad : connectedRoadsCopy) {
			if (fromRoad) {
				std::vector<std::shared_ptr<RoadSegment>> group;
				group.push_back(fromRoad);
				phaseGroups.push_back(group);
			}
		}

		phases.clear();
		for (const auto& group : phaseGroups) {
			TrafficPhase phase;
			phase.duration = 5.0f;

			for (const auto& fromRoad : group) {
				// skip invalid roads
				if (!fromRoad) continue;
				for (auto& toRoad : connectedRoadsCopy) {
					if (!toRoad) continue; 
					if (fromRoad->getId() != toRoad->getId()) {
						phase.allowedMovements.push_back({ fromRoad->getId(), toRoad->getId() });
					}
				}
			}

			// add phase
			if (!phase.allowedMovements.empty()) {
				phases.push_back(phase);
			}
		}

		// create a default
		if (phases.empty() && !connectedRoadsCopy.empty()) {
			TrafficPhase defaultPhase;
			defaultPhase.duration = 5.0f;
			phases.push_back(defaultPhase);
		}
	}


	Vector3 getEntryPoint(std::shared_ptr<RoadSegment> road) const {
		auto startJunc = road->getStartJunction();
		auto endJunc = road->getEndJunction();

		if (startJunc && startJunc->getId() == this->getId()) {
			return road->getStartPosition();
		} else if (endJunc && endJunc->getId() == this->getId()) {
			return road->getEndPosition();
		}

		return this->getPosition();
	}


	LightState getLightState(std::shared_ptr<RoadSegment> road) const {
		if (!road) {
			return LightState::RED;
		}

		int roadIndex = getRoadIndex(road);
		if (roadIndex >= 0 && roadIndex < static_cast<int>(trafficLights.size())) {
			return trafficLights[roadIndex].state;
		}
		return LightState::RED;
	}


private:

	struct TrafficPhase {
		std::vector<std::pair<std::string, std::string>> allowedMovements;
		float duration;
	};

	struct TrafficLight {
		LightState state;
		float timer;
		int roadIndex;
	};

	std::vector<TrafficPhase> phases;
	std::vector<TrafficLight> trafficLights;

	int currentPhase;
	float phaseTimer;

	int currentGreenLight;
	float greenDuration;
	float yellowDuration;
};