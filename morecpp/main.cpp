#include <random>
#include <iostream>

#include "core/vec3.h"
#include "road/roadSegment.h"
#include "road/roadNetwork.h"
#include "road/simpleJunction.h"
#include "road/spawnPoint.h"
#include "navigation/destination.h"
#include "traffic/car.h"
#include "ui/openGLGameLoop.h"

int main() {
    RoadNetwork roadNetwork;
    roadNetwork.buildNetwork(2, 2, 400.0f, 32.0f, 10.0f);

    OpenGLGameLoop::runNetwork(roadNetwork);

    return 0;
}
