#include <iostream>

#include "ui/openGLGameLoop.h"
#include "framework/simulationController.h"


int main() {
    SimulationController controller;
    controller.init();
    controller.runCustomNetworkSimulation();

    return 0;
}