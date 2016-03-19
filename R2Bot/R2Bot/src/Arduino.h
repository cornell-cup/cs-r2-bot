#pragma once

#include "BaseConnection.h"
#include <mutex>

namespace R2D2 {

	struct PowerData {
		float batteryPercent;
		float voltage;
		float regulatedCurrent;
		float unregulatedCurrent;
	};

	class Arduino {
	public:
		Arduino(const std::string& comPort);
		PowerData getPowerData();
		ImuData getImuData();

	private:
		Serial serial;
		std::mutex lock;
	};

	class PowerMonitor {
	public:
		PowerMonitor(Arduino& arduino, BaseConnection& base);
		void operator()();

	private:
		Arduino& arduino;
		BaseConnection& baseConn;
	};

	class ImuMonitor {
	public:
		ImuMonitor(Arduino& arduino);
		void operator()();

	private:
		Arduino& arduino;
	};

}