#pragma once

class SensorData {
protected:

public:
	SensorData(bool data);
	~SensorData();

	/**
	 * Sensor availability
	 */
	bool hasData;
};
