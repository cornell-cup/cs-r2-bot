#include "stdafx.h"
#include "HighwayData.h"

HighwayData::HighwayData(bool inData, bool inDetected, long inDetectionTime) : 
	SensorData(inData), detected(inDetected), detectionTime(inDetectionTime) {

}

HighwayData::~HighwayData() {

}