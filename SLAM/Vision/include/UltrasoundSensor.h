#pragma once
#include "Sensor.h"

class UltrasoundSensor : public Sensor {
protected:
	// Private/protected variables here
	bool initialized;

	std::string port;

	HANDLE usHandle;

	OVERLAPPED readoverlap;
public:
	// Public variables here

	//array Gives locations for each sensor
	std::vector<std::string> locations;

	//Constructor
	UltrasoundSensor(std::string inPort, std::vector<std::string> inLocationList);

	//Destructor
	~UltrasoundSensor();

	/**
	 * Get data from the ultrasound sensors.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
};
