#pragma once
#include "SensorData.h"

class HighwayData : public SensorData {
protected:
	// Private/protected variables here
public:
	HighwayData(bool hasData = false, bool inDetected = false, long inDetectionTime = 0);
	~HighwayData();

	/*
	* Highway line detected or not
	*/
	bool detected;
	/*
	* Amount of time highway is detected
	*/
	long detectionTime;
};
