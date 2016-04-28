#include "stdafx.h"
#include "LidarData.h"

LidarData::LidarData(bool inData, std::vector<double> inAngles, std::vector<double> inDistances) :
	SensorData(inData), angles(inAngles), distances(inDistances) {

}

LidarData::~LidarData() {
	
}