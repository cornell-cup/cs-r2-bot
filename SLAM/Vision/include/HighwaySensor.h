#pragma once
#include "Sensor.h"

class HighwaySensor : public Sensor {
protected:
	// Private/protected variables here
public:
    // Public variables here

    //Constructor
    HighwaySensor();

    //Destructor
    ~HighwaySensor();

    /**
	 * Get data from the highway rgb sensor.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
};
