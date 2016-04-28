#pragma once
#include "Sensor.h"
#include "easywsclient.hpp"

class PhoneIMUSensor : public Sensor {
protected:
	// Private/protected variables here

	// Latest values
	double accX, accY, accZ;
	double rotA, rotB, rotC;

	/**
	 * Handle a websocket message
	 */
	void handle_message(const std::string & message);
public:
	// Public variables here

	// Client connection
	easywsclient::WebSocket::pointer wsc;

	PhoneIMUSensor();
	~PhoneIMUSensor();

	/**
	* Get data from the phone.
	* @param sdata		The pointer to the sensor data bag.
	*/
	void getData(SensorDataBag * sdata);
};
