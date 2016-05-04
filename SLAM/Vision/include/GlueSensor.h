#pragma once
#include "Sensor.h"
#include "easywsclient.hpp"

// class defines an insttance of a sensor
class GlueSensor : public Sensor {
protected:
	// Private/protected variables here
	KinectData * tmpKinectData;


public:
	// Public variables here

	std::string name; // name of sensor
	int id; // unique identifier
	static int count; // Number of sensors (used for ID)

	// Client connection
	easywsclient::WebSocket::pointer wsc;

	/**
	* Sensor constructor.
	*/
	GlueSensor(std::string inName);

	/**
	* Sensor destructor.
	*/
	~GlueSensor();

	/**
	* If this sensor is available, getData fills the corresponding SensorDataBag member with data from this sensor.
	* @param sdata		The pointer to the sensor data bag.
	*/
	void getData(SensorDataBag * sdata);

};
