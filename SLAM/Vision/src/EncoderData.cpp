#include "stdafx.h"
#include "EncoderData.h"

EncoderData::EncoderData(bool inData, int inLeftMotor, int inRightMotor, 
	int inLastLeftMotor, int inLastRightMotor) : SensorData(inData), leftMotor(inLeftMotor), 
	rightMotor(inRightMotor), lastLeftMotor(inLastLeftMotor), lastRightMotor(inLastRightMotor) {

}

EncoderData::~EncoderData() {

}