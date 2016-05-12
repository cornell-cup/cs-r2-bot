#include "stdafx.h"
#include "KinectData.h"

KinectData::KinectData(bool inData, std::vector<double> inObstacleX, std::vector<double> inObstacleY)
	: SensorData(inData), obstacleX(inObstacleX), obstacleY(inObstacleY) {

}

KinectData::~KinectData() {

}
