#pragma once

#include "EncoderData.h"
#include "HighwayData.h"
#include "IMUData.h"
#include "KinectData.h"
#include "LidarData.h"
#include "UltrasoundData.h"

/**
 * Contains sensor data for all available sensor types.
 */
class SensorDataBag {
protected:

public:
	/**
	 * Initialize a new sensor data bag.
	 */
	SensorDataBag();

	/**
	 * Delete all sensor data.
	 */
	~SensorDataBag();

	/**
	 * Clean up sensor data without deleting the bag.
	 */
	void clean();

	/**
	 * Encoder data
	 */
	EncoderData * encoder;

	/**
	 * Highway detection data
	 */
	HighwayData * highway;

	/**
	 * IMU data
	 */
	IMUData * imu;

	/**
	 * Kinect data
	 */
	KinectData * kinect;

	/**
	 * Lidar sensor
	 */
	LidarData * lidar;

	/**
	 * Ultrasound data
	 */
	UltrasoundData * ultrasound;
};
