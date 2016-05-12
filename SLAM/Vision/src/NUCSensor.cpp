#include "stdafx.h"
#include "NUCSensor.h"

NUCSensor::NUCSensor(std::string inAddress, int inPort) : Sensor("NUC") {
	server = new UDPSocketServer(inAddress, inPort);
	if (server->isListening()) {
		server->server([this](char * buffer, unsigned int buffer_len) {
			double x = 0.0;
			double y = 0.0;
			double height = 0.0;
			if (sscanf_s(buffer, "[%lf,%lf,%lf]", &x, &y, &height) == 3) {
				if (obstacleX.size() <= 640) {
					if (height < 0.01 && height > -0.01) {
						obstacleX.push_back(x);
						obstacleY.push_back(y);
					}
				} 
			}
		});
	}
}

NUCSensor::~NUCSensor() {
	// TODO clean up
}

void NUCSensor::getData(SensorDataBag * sdata) {
	sdata->kinect = new KinectData(true, obstacleX, obstacleY);
	//Clear obstacle arrays
	obstacleX.clear();
	obstacleY.clear();
}
