#include "stdafx.h"
#include "EncoderSensor.h"

EncoderSensor::EncoderSensor(std::string inPort) : Sensor("Encoder"), port(inPort), leftMotor(0), rightMotor(0), deltaLeftMotor(0), deltaRightMotor(0) {
	// TODO connect to the sensor here
}

EncoderSensor::~EncoderSensor() {
	// TODO clean up
}

void EncoderSensor::getData(SensorDataBag * sdata) {
	// TODO fill sdata.encoder with proper data
}
