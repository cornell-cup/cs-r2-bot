#pragma once
#include "Sensor.h"
#include "easywsclient.hpp"

class IMUSensor : public Sensor {
protected:
	// Private/protected variables here
	// Client connection
	easywsclient::WebSocket::pointer wsc;
    
	// Latest values
    unsigned long lastTime;
    double dt;
	double accX, accY, accZ;
	double rotA, rotB, rotC;
public:
	// Public variables here

    //constructor
    IMUSensor();

    //destructor
    ~IMUSensor();

	/**
	 * Get data from the IMU.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
};
