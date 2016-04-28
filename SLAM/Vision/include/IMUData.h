#pragma once
#include "SensorData.h"

class IMUData : public SensorData {
protected:
	// Private/protected variables here
public:
	IMUData(bool inData = false, double inDt = 0.0, double inAccelX = 0.0, double inAccelY = 0.0,
        double inAccelZ = 0.0, double inGyroX = 0.0, double inGyroY = 0.0, double inGyroZ = 0.0);
	~IMUData();

    // DT
    double dt;
    
	/*
	* X, Y, Z acceleration of R2
	*/
	double accelX;
	double accelY;
	double accelZ;
	/*
	* X, Y, Z orientation of R2
	*/
	double gyroX;
	double gyroY;
	double gyroZ;
};
