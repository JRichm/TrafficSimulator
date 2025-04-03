#pragma once

#include <vector>
#include "vec3.h"
#include "color.h"


class GameObject {
protected:
	Vector3 position;
	Vector3 dimensions;
	Color color;


public:
	GameObject(const Vector3& pos, const Vector3& dim, const Color& col)
		: position(pos), dimensions(dim), color(col) {}

	virtual ~GameObject() = default;

	virtual void update(float deltaTime) = 0;

	// getters/setters
	const Vector3& getPosition() const { return position; }
	void setPosition(const Vector3& pos) { position = pos; }
	const Vector3& getDimensions() const { return dimensions; }
	const Color& getColor() const { return color; }
};
