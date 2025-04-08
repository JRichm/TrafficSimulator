#pragma once

#include "simulationModel.h"
#include "viewController.h"


class simulationController {
private:
	SimulationModel model;
	ViewController view;

	bool running = false;
	float lastFrameTime = 0.0f;
	int frameCount = 0;
	int maxFrames = 0;

public:
	simulationController();

	void init();
	void run(int maxIterations = 0);
	void stop();
	void runCustomNetworkSimulation();
	void runGridNetwrokSimulation(int width, int height);
};

