#pragma once

#include <vector>
#include "vec3.h"


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
	Lane(int idx, LaneType type, float width);

	int getIndex() const;
	LaneType getType() const;
	float getWidth() const;
	bool isReversible() const;
	void setReversible(bool reversible);

	bool canAcceptVehicle(Vehicle* vehicle) const;
};