# Traffic Simulator
A C++ traffic simulation application that models vehicle behavior on a multi-lane road using OpenGL for visualization

## Overview
This traffic simulator creates a virtual road environment where vehicles move with realistic behaviors including

- Lane changing decisions
- Speed adjustments based on surrounding traffic
- Continuous simulation with vehicle respawning

## Features

- 🚗 Multiple vehicles with individual behaviors
- 🛣️ Multi-lane road system
- 🚦 Intelligent traffic behaviors (speed adjustment, lane changing)
- 🔄 Continuous simulation with vehicle respawning
- 🎮 Real-time visualization using OpenGL
- 🎨 Color-coded vehicles for easy tracking

## How It Works
The simulation uses a time-step based physics system:

- Position updates based on velocity and time
- Continuous collision detection and avoidance
- Decision-making algorithms for lane changes

## Requirements

- C++11 or higher
- OpenGL 3.3 or higher
- GLFW
- GLAD
- GLM

## Screenshots

![alt text](https://github.com/JRichm/TrafficSimulator/blob/main/screenshots/250330.png)
![alt text](https://github.com/JRichm/TrafficSimulator/blob/main/screenshots/250403.png)
![alt text](https://github.com/JRichm/TrafficSimulator/blob/main/screenshots/250405.png)
