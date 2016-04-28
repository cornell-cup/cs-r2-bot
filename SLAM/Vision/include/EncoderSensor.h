#pragma once
#include "Sensor.h"

class EncoderSensor : public Sensor {
protected:
	// Private/protected variables here
public:
	// Public variables here

	/**
	 * The serial port connected to the sensor.
	 */
	std::string port;
	/**
	 * Absoltue encoder data from the left and right motors.
	 */
	int leftMotor, rightMotor;
	/**
	 * Relative encoder data from the left and right motors.
	 */
	int deltaLeftMotor, deltaRightMotor;

	/**
	 * Initializes a connection to the encoder sensors.
	 */
	EncoderSensor(std::string inPort);

	/**
	 * Encoder sensor destructor.
	 */
	~EncoderSensor();

	/**
	 * Get data from the encoders.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
};
