#include "stdafx.h"
#include "HighwaySensor.h"

HighwaySensor::HighwaySensor() : Sensor("RGB Highway") {
	// TODO connect to the sensor here
}

HighwaySensor::~HighwaySensor() {
	// TODO clean up
}

void HighwaySensor::getData(SensorDataBag * sdata) {
	// TODO fill sdata.highway with proper data
}