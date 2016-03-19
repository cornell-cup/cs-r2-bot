#include "stdafx.h"
#include "Fpga.h"

namespace R2D2 {

	const DWORD INVENTORY_SENSOR = 10;
	const double METERS_PER_RANGEFINDER_UNIT = 1.0 / 100.0;
	// Stop thresholds for rangefinders. If a rangefinder sees an object closer
	// than a corresponding value in this array, then the bot is forced to stop.
	const double RANGEFINDER_FORCE_STOP_TOLERANCES[N_RANGEFINDERS] =
		{ 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };
	const int N_AVERAGE_READINGS = 10;

	Fpga::Fpga() {
		libHandle = PCIE_Load();
		if (!libHandle) {
			throw std::runtime_error("Failed to load PCIE lib");
		}

		pcieHandle = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
		if (!pcieHandle) {
			PCIE_Unload(libHandle);
			throw std::runtime_error("Failed to open PCIE");
		}
	}

	Fpga::~Fpga() {
		PCIE_Close(pcieHandle);
		PCIE_Unload(libHandle);
	}

	DWORD Fpga::request(DWORD sensorId) {
		std::unique_lock<std::mutex>(this->lock);

		// Write request
		BOOL pass = PCIE_Write32(pcieHandle, PCIE_BAR0, 0x00, sensorId);
		if (!pass) { throw std::runtime_error("PCIE write failed"); }
		
		// Receive response
		DWORD input;
		pass = PCIE_Read32(pcieHandle, PCIE_BAR0, 0x20, &input);
		if (!pass) { throw std::runtime_error("PCIE write failed"); }

		return input;
	}

	InventoryMonitor::InventoryMonitor(Fpga& f, BaseConnection& base) : fpga(f), baseConn(base) { }

	void InventoryMonitor::operator()() {
		baseConn.updateInventory(fpga.request(10));
	}

	
	RangeFinderMonitor::RangeFinderMonitor(Fpga& f, WheelController& wctrl)
			: fpga(f), wheelCtrl(wctrl), nReadings(0) {
		std::memset(averages, 0, sizeof(averages));
	}

	void RangeFinderMonitor::operator()() {
		bool enableWheels = true;
		for (int i = 0; i < N_RANGEFINDERS; i++) {
			DWORD sensorId = i + 1;
			double distance = METERS_PER_RANGEFINDER_UNIT * fpga.request(sensorId);
			if (i == 3) Log::trace("Rangefinder %d: %f", sensorId, distance);
			if (nReadings < N_AVERAGE_READINGS) {
				averages[i] += distance / N_AVERAGE_READINGS;
				//Log::trace("Average is %f", averages[i]);
			} /*else if (abs(distance - averages[i]) >= RANGEFINDER_FORCE_STOP_TOLERANCES[i]) {
				enableWheels = false;
				Log::info("Rangefinder %d threshold reached; stopping motors", sensorId);
			}*/
		}
		wheelCtrl.setEnabled(enableWheels);
		if (nReadings < N_AVERAGE_READINGS) { nReadings++; }
	}

}