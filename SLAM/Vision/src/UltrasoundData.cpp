#include "stdafx.h"
#include "UltrasoundData.h"

UltrasoundData::UltrasoundData(bool inData, int inNumSensors, double * inDistances) : 
	SensorData(inData), numSensors(inNumSensors), distances(inDistances) {

}

UltrasoundData::~UltrasoundData() {
	delete distances;
}
