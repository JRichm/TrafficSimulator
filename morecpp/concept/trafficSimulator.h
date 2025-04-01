#pragma once

#include <memory>
#include "roadNetwork.h"
#include "openglDisplay.h"


class TrafficSimulator {
private:
	std::shared_ptr<RoadNetwork> network;
	OpenGLDisplay display;

	float simulationSpeed;
	bool isPaused;


public:
	TrafficSimulator();
	void loadNetwork(const std::string& filename);
	void run();

	void setSimulationSpeed(float speed);
	void togglePause();

	struct TrafficStats {
		float averageSpeed;
		int vehicleCount;
		int trafficJams;
		int carCollisions;
	};

	TrafficStats gatherStatistics() const;
};