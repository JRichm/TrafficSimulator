#include <iostream>

#include "framework/simulationController.h"


int main() {
    SimulationController controller;
    controller.init();
    controller.runGridNetwrokSimulation(3, 3);

    return 0;
}