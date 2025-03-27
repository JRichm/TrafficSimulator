#pragma once
#include "vector3.h"
#include "color.h"
#include "boundingbox.h"

class GameObject {
protected:
    Vector3 position;
    Vector3 dimensions;
    Color color;
    BoundingBox bounds;

public:
    GameObject(const Vector3& pos, const Vector3& dim, const Color& col)
        : position(pos), dimensions(dim), color(col) {
        updateBounds();
    }

    virtual void update(float deltaTime) = 0;  // Pure virtual update method

    Vector3 getPosition() const { return position; }
    Vector3 getDimensions() const { return dimensions; }
    Color getColor() const { return color; }
    BoundingBox getBounds() const { return bounds; }

protected:
    void updateBounds() {
        bounds.min = position;
        bounds.max = position + dimensions;
    }
};