#pragma once

#include "simulationModel.h"
#include "viewController.h"


class SimulationController {
private:
	SimulationModel model;
	ViewController view;

	bool running = false;
	float lastFrameTime = 0.0f;
	int frameCount = 0;
	int maxFrames = 0;

public:
	SimulationController();

	void init();
	void run(int maxIterations = 0);
	void stop() { running = false; }
	void runCustomNetworkSimulation();
	void runGridNetwrokSimulation(int width, int height);
};
