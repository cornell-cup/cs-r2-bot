#pragma once
#include <thread>
#include <mutex>
#include "Sensor.h"
#include "rplidar.h"

#define NODE_COUNT 360 * 2

class LidarSensor : public Sensor {
protected:
	// RPLidar driver
	rp::standalone::rplidar::RPlidarDriver * driver;

	// Saved scan data
	unsigned int node_count = NODE_COUNT;
	std::vector<double> angles;
	std::vector<double> distances;
	// Scan data lock
	std::mutex scan_mutex;

	// Background scanning thread
	std::thread bg_thread;
	bool bg_done = false;
	// Background scanning function
	void bg_scan();
public:
	// Public variables here

	// The serial port connected to the sensor
	std::string port;

	LidarSensor(std::string inPort);

	// Destructor to destroy the instance
	~LidarSensor();

	/**
	 * Get data from the LIDAR.
	 * @param sdata		The pointer to the sensor data bag.
	 */
	void getData(SensorDataBag * sdata);
};
