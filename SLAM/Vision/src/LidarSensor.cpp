#include "stdafx.h"
#include "LidarSensor.h"
#include <algorithm>

using namespace rp::standalone::rplidar;

LidarSensor::LidarSensor(std::string inPort) : Sensor("LIDAR"), port(inPort), driver(NULL) {
	// Connect to the lidar
	driver = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
	if (!driver) {
		printf("Failed to create RPLIDAR driver\n");
	}
	if (IS_FAIL(driver->connect(port.c_str(), 115200))) {
		printf("Failed to connect to RPLIDAR\n");
	}
	// Wait for a while, then start scanning
	Sleep(1000);
	if (IS_FAIL(driver->startScan())) {
		printf("Failed to start RPLIDAR scanning\n");
	}

	// Start a background thread for scanning
	bg_thread = std::thread(&LidarSensor::bg_scan, this);
	bg_thread.detach();
}

LidarSensor::~LidarSensor() {
	bg_done = true;
	RPlidarDriver::DisposeDriver(driver);
}

void LidarSensor::bg_scan() {
	u_result result;

	while (!bg_done) {
		rplidar_response_measurement_node_t scan_nodes[NODE_COUNT];
		node_count = NODE_COUNT;

		result = driver->grabScanData(scan_nodes, node_count);

		if (IS_OK(result) || result == RESULT_OPERATION_TIMEOUT) {
			driver->ascendScanData(scan_nodes, node_count);

			// Set nodes to newly scanned nodes
			scan_mutex.lock();
			angles.clear();
			distances.clear();
			for (int i = 0; i < node_count; i++) {
				double angle = (scan_nodes[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0;
				double distance = scan_nodes[i].distance_q2 / 4000.0f;
				int quality = scan_nodes[i].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;

				if (quality >= 10) {
					angles.push_back(angle);
					distances.push_back(distance);
				}
			}
			scan_mutex.unlock();
		}
	}
}

void LidarSensor::getData(SensorDataBag * sdata) {
	sdata->lidar = new LidarData(true);
	scan_mutex.lock();
	unsigned int l = std::min(angles.size(), distances.size());
	for (int i = 0; i < l; i++) {
		sdata->lidar->angles.push_back(angles[i]);
		sdata->lidar->distances.push_back(distances[i]);
	}
	scan_mutex.unlock();
}
