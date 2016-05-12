#pragma once
#include "SensorData.h"

class KinectData : public SensorData {
protected:
	// Private/protected variables here
public:
	KinectData(bool hasData, std::vector<double> obstacleX, std::vector<double> obstacleY);
	~KinectData();

	/**
	 * Array of x coordinates of obstacles
	 */
	std::vector<double> obstacleX;
	/**
	 * Array of y coordinates of obstacles
	 */
	std::vector<double> obstacleY;
};
