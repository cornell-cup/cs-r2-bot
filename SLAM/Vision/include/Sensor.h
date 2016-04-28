#pragma once
#include "SensorData.h"
#include "SensorDataBag.h"

// class defines an insttance of a sensor
class Sensor {
protected:
	// Private/protected variables here

public:
	// Public variables here
    
    std::string name; // name of sensor
	int id; // unique identifier
	static int count; // Number of sensors (used for ID)
	
    /**
	 * Sensor constructor.
	 */
	Sensor(std::string inName);

	/**
	 * Sensor destructor.
	 */
	virtual ~Sensor();

	/**
	 * If this sensor is available, getData fills the corresponding SensorDataBag member with data from this sensor.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	virtual void getData(SensorDataBag * sdata);
  
};
