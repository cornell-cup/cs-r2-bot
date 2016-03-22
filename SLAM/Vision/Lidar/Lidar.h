#pragma once

#include "Serial.h"
#include <vector>

struct PolarPoint {
	int angle;  // In degrees
	int depth;
};

class Lidar {
public:
	Lidar(const char* comPort);
	std::vector<PolarPoint> readData();
private:
	Serial device;
};