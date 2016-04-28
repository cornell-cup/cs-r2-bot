#pragma once
#include "SensorData.h"

class EncoderData : public SensorData {
protected:
	// Private/protected variables here
public:
	EncoderData(bool hasData = false, int inLeftMotor = 0, int inRightMotor = 0, int inLastLeftMotor = 0, int inLastRightMotor = 0);
	~EncoderData();

	/*
	* Value of the left motor
	*/
	int leftMotor;
	/*
	* Value of the right motor
	*/
	int rightMotor;
	/*
	* Last value of the left motor
	*/
	int lastLeftMotor;
	/*
	* Last value of the right motor
	*/
	int lastRightMotor;
};
