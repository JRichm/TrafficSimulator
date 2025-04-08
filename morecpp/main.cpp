#include <iostream>

#include "framework/simulationController.h"


int main() {
    SimulationController controller;
    controller.init();
    controller.runCustomNetworkSimulation();

    return 0;
}