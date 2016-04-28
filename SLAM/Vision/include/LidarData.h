#pragma once
#include "SensorData.h"

class LidarData : public SensorData {
protected:
	// Private/protected variables here
public:
	LidarData(bool inData = false, std::vector<double> inAngles = std::vector<double>(), std::vector<double> inDistances = std::vector<double>());
	~LidarData();

	/*
	* Array of angles for each point returned
	*/
	std::vector<double> angles;
	/*
	* Array of distances for each point returned. 
	* Same indexes of angles[] and distance[] correspond to same point
	*/
	std::vector<double> distances;
};
