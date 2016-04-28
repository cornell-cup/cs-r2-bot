#include "stdafx.h"
#include "UltrasoundSensor.h"

UltrasoundSensor::UltrasoundSensor(std::string inPort, std::vector<std::string> inLocationList) : Sensor("Ultrasound"), initialized(false), port(inPort), locations(inLocationList) {
	
}

UltrasoundSensor::~UltrasoundSensor(){
	
}

void UltrasoundSensor::getData(SensorDataBag * sdata) {
	
}
