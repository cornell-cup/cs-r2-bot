#pragma once
#include "Sensor.h"
#include "KinectData.h"
#include "KinectInterface.h"
#include "Kinect1.h"

class KinectSensor : public Sensor {
protected:
	// Private/protected variables here
	KinectInterface * kinect;

	UINT16 * lastData;
public:
	// Public variables here

    //Constructor
    KinectSensor();

    //Destructor
    ~KinectSensor ();

    /**
	 * Get data from the Kinect.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
    
    
};
