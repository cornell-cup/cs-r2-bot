#pragma once
#include "Sensor.h"
#include "UDPSocketServer.h"

// class defines an insttance of a sensor
class NUCSensor : public Sensor {
protected:
	// UDP Server
	UDPSocketServer * server;

	// Private/protected variables here
	KinectData * tmpKinectData;

public:
	// Public variables here

	std::string name; // name of sensor
	int id; // unique identifier
	static int count; // Number of sensors (used for ID)
	std::vector<double> obstacleX, obstacleY;

	/**
	* Sensor constructor.
	*/
	NUCSensor(std::string inAddress, int inPort);

	/**
	* Sensor destructor.
	*/
	~NUCSensor();

	/**
	* If this sensor is available, getData fills the corresponding SensorDataBag member with data from this sensor.
	* @param sdata		The pointer to the sensor data bag.
	*/
	void getData(SensorDataBag * sdata);

};
