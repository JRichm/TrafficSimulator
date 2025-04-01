#pragma once

#include <chrono>
#include <thread>
#include "trafficSimulator.h"


class GameLoop {
public:
	static void run(TrafficSimulator& simulator, int maxIterations = 0);
};