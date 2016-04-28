#include "stdafx.h"
#include "KinectData.h"

KinectData::KinectData(bool inData, UINT32 * inCamera, UINT16 * inDepth, double inDiff)
	: SensorData(inData), camera(inCamera), depth(inDepth), diff(inDiff) {

}

KinectData::~KinectData() {

}
