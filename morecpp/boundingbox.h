#pragma once
#include "vector3.h"

class BoundingBox {
public:
    Vector3 min, max;

    BoundingBox() = default;
    BoundingBox(const Vector3& min, const Vector3& max) : min(min), max(max) {}

    bool intersects(const BoundingBox& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }
};