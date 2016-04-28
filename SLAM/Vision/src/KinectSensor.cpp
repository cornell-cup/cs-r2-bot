#include "stdafx.h"
#include "KinectSensor.h"

KinectSensor::KinectSensor() : Sensor("Kinect"), lastData(new UINT16[640 * 480]) {
	// Setup Kinect
	kinect = new Kinect1();
}

KinectSensor::~KinectSensor() {
	delete kinect;
}

double dabs(double val) {
	return (val < 0) ? -val : val;
}

void KinectSensor::getData(SensorDataBag * sdata) {
	UINT32 * colorBuffer = kinect->getColorBuffer();
	UINT16 * depthBuffer = kinect->getDepthBuffer();

	UINT16 * depthPtr = depthBuffer;
	UINT16 * dataPtr = lastData;
	int pixels = 640 * 480;
	double diff = 0.0;
	while (pixels-- > 0) {
		diff = diff + dabs((*dataPtr) - (*depthPtr));
		(*dataPtr) = (*depthPtr);
		dataPtr++;
		depthPtr++;
	}

	sdata->kinect = new KinectData(true, colorBuffer, depthBuffer, diff / (640.0 * 480.0));
	//printf("%lf\n", (diff / (640.0 * 480.0)));
}

