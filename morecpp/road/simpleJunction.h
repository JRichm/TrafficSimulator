#pragma once

#include "junction.h"


class SimpleJunction : public Junction {
public:
    SimpleJunction(const std::string& id, const Vector3& pos)
        : Junction(id, pos) {}

    bool canNavigate(std::shared_ptr<RoadSegment> fromRoad, std::shared_ptr<RoadSegment> toRoad, Vehicle* vehicle) override {
        return true;
    }
};