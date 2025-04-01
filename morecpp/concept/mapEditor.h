#pragma once

#include <vector>
#include <memory>
#include <string>
#include "roadNetwork.h"
#include "openglDisplay.h"


enum class EditorTool {
	SELECT,
	ROAD,
	JUNCTION,
	SPAWN,
	DESTINATION,
	TRAFFIC_LIGHT
};


class MapEditor {
private:
	std::shared_ptr<RoadNetwork> network;
	OpenGLDisplay display;
	EditorTool currentTool;

	// state
	bool isPlacing;
	Vector3 startPoint;
	Vector3 endpoint;

	// selcted objects
	std::shared_ptr<GameObject> selectedObject;

	// controls
	void handleClick(double xpos, double ypos);
	void handleDrag(double xpos, double ypos);
	void handleRelease(double xpos, double ypos);
	void handleKeyPress(int key);


public:
	MapEditor();
	void run();

	void setTool(EditorTool tool);
	void saveNetwork(const std::string& filename);
	std::shared_ptr<RoadNetwork> loadNetwork(const std::string& filename);
};