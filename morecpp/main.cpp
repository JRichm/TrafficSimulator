#include <iostream>

#include "framework/simulationController.h"


int main() {
    SimulationController controller;
    controller.init();
    controller.runGridNetwrokSimulation(2, 2, 3);

    return 0;
}