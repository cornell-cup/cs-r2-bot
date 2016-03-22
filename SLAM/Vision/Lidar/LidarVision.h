#pragma once

#include "Lidar.h"
#include "BotComm/R2Comm.h"
#include "BotComm/VisionComm.h"
#include <vector>
#include <memory>

class LidarVision {
public:
	LidarVision(const char* comPort, R2D2::BotPosition& pos);
	int grid[GRID_HEIGHT][GRID_WIDTH];

private:
	Lidar lidar;
	R2D2::BotPosition& botPos;
};