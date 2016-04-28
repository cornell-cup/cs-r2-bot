#include "stdafx.h"
#include "Sensor.h"

// TODO Sensor.cpp and Sensor.h

int Sensor::count = 0;

Sensor::Sensor(std::string inName) : name(inName), id(Sensor::count) {
	Sensor::count++;
}

// destroys the sensor object
Sensor::~Sensor() {

}

void Sensor::getData(SensorDataBag * sdata) {
	// TODO
}
