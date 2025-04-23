#include <iostream>

#include "framework/simulationController.h"


int main() {
    SimulationController controller;
    controller.init();
    controller.runGridNetwrokSimulation(6, 1, 6);

    return 0;
}