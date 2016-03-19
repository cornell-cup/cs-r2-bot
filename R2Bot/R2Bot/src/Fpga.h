#pragma once

#include "PCIE/PCIE.h"
#include "BaseConnection.h"
#include <mutex>

namespace R2D2 {

	const int N_RANGEFINDERS = 9;

	class Fpga {
	public:
		Fpga();
		~Fpga();
		DWORD request(DWORD sensorId);

	private:
		void* libHandle;
		PCIE_HANDLE pcieHandle;
		std::mutex lock;
	};

	class InventoryMonitor {
	public:
		InventoryMonitor(Fpga& f, BaseConnection& base);
		void operator()();

	private:
		Fpga& fpga;
		BaseConnection& baseConn;
	};

	class RangeFinderMonitor {
	public:
		RangeFinderMonitor(Fpga& f, WheelController& wctrl);
		void operator()();

	private:
		Fpga& fpga;
		WheelController& wheelCtrl;
		int nReadings;
		double averages[N_RANGEFINDERS];
	};

}