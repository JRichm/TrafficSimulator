#pragma once

#include <vector>

#include "../core/vec3.h"


enum class LaneType {
	REGULAR,
	EXIT_ONLY,
	ENTRANCE_ONLY,
	HOV,
	SHOULDER,
	TURN_LEFT,
	TURN_RIGHT
};


// forward declaration
class Vehicle;


class Lane {
private:
	int index;
	LaneType type;
	float width;
	bool isReversible;


public:
	Lane(int idx, LaneType type, float width) : index(idx), type(type), width(width), isReversible(false) {}

	int getIndex() const { return index; }
	LaneType getType() const { return type; }
	float getWidth() const { return width; }
	bool isReversible() const { return isReversible; }
	void setReversible(bool reversible) { isReversible = reversible; }

	bool canAcceptVehicle(Vehicle* vehicle) const;
};