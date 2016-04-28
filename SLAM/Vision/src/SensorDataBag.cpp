#include "stdafx.h"
#include "SensorDataBag.h"

SensorDataBag::SensorDataBag() {

}

SensorDataBag::~SensorDataBag() {
	clean();
}

void SensorDataBag::clean() {
	delete encoder;
	delete highway;
	delete imu;
	delete kinect;
	delete lidar;
	delete ultrasound;
}
