#pragma once
#include "SensorData.h"

class UltrasoundData : public SensorData {
protected:
	// Private/protected variables here
public:
	UltrasoundData(bool hasData = false, int inNumSensors = 0, double * inDistances = NULL);
	~UltrasoundData();

	/*
	 * Number of sensors
	 */
	int numSensors;
	/*
	* Array of distances for each individual ultrasound sensor
	*/
	double * distances;
};
