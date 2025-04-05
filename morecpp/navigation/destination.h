#pragma once


#include <string>

#include "../core/vec3.h"


class Destination {
private:
	Vector3 position;
	std::string name;
	float radius;

	
public:
	Destination(const Vector3& pos, const std::string& name, float radius = 10.0f) : position(pos), name(name), radius(radius) {}

	const Vector3& getPosition() const { return position; }
	const std::string& getName() const { return name; }
	float getRadius() const { return radius; }

	bool isInRange(const Vector3& pos) const {
		Vector3 dis = pos - position; 

		return dis.length() <= (radius * radius);
	}
};