#include <random>

#include "core/vec3.h"
#include "road/roadSegment.h"
#include "road/simpleJunction.h"
#include "traffic/car.h"
#include "ui/openGLGameLoop.h"

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> speedDist(3.0f, 12.0f);

	auto road = std::make_shared<RoadSegment>("main_road", Vector3(0, 0, 0), Vector3(1024.0f, 32.0f, 0), 12.0f);
	road->addLane(Lane(0, LaneType::SHOULDER, 2.0f));
	road->addLane(Lane(1, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(2, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(3, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(4, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(5, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(6, LaneType::REGULAR, 4.0f));
	road->addLane(Lane(7, LaneType::SHOULDER, 2.0f));

	auto endJunction = std::make_shared<SimpleJunction>("junction1", Vector3(1200.f, 0, 0));
	road->setJunctions(nullptr, endJunction);

	for (int i = 0; i < 50; i++) {
		int lane = gen() % 3;
		float offset = 35.0f + i * 10.0f;

		float lanePos = (lane - 1) * 4.0f;

		auto car = std::make_shared<Car>(
			Vector3(offset, lanePos, 0),
			Vector3(4, 2, 1),
			Color(gen() & 200 + 55, gen() % 200 + 55, gen() % 200 + 55),
			speedDist(gen)
		);

		road->addVehicle(car);
	}

	OpenGLGameLoop::run(*road);

	return 0;
}
