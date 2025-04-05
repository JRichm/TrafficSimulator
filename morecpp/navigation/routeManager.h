#pragma once


#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "../road/roadSegment.h"
#include "../navigation/destination.h"


class RouteManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Junction>> junctions;
    std::unordered_map<std::string, std::shared_ptr<RoadSegment>> roadSegments;


public:
    void addJunction(std::shared_ptr<Junction> junction) {
        junctions[junction->getId()] = junction;
    }

    void addRoadSegment(std::shared_ptr<RoadSegment> roadSegment) {
        roadSegments[roadSegment->getId()] = roadSegment;
    }
};
