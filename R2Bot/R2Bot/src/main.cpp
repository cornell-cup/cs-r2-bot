#include "stdafx.h"
#include "Config.h"
#include "BaseToBotImpl.h"
#include "BaseConnection.h"
#include "Vision.h"
#include "PeriodicTimer.h"
#include "PositionMonitor.h"
#include "Arduino.h"
#include "Fpga.h"
#include "VisionCommImpl.h"
#include "Util.h"

using namespace R2D2;

const char CONFIG_FILE[] = "config.txt";

// Period between updates to Base (in milliseconds)
const int WHEEL_POS_UPDATE_PERIOD = 200;
const int POWER_UPDATE_PERIOD = 2000;
const int IMU_UPDATE_PERIOD = 500;
const int INVENTORY_UPDATE_PERIOD = 2000;
const int RANGEFINDER_UPDATE_PERIOD = 200;

int _tmain(int argc, _TCHAR* argv[]) {
	Log::trace("Program started");

	Config config = Config::fromFile(CONFIG_FILE);
	Log::level = config.verbose;

	while (true) {
		try {
			// IO service used for async callbacks.
			boost::asio::io_service service;

			// Initialize system components.

			WheelController wctrl(config.wheelsComPort);
			HeadController hctrl(config.headComPort, config.flapComPort);

			BaseToBotImpl* baseToBot = new BaseToBotImpl(wctrl, hctrl, config.connTimeout, service);
			BaseConnection baseConn(baseToBot, config.botPort, config.basePort);

			Arduino arduino(config.arduinoComPort);
			//PowerMonitor powerMonitor(arduino, baseConn);
			//PeriodicTimer<PowerMonitor> powerTimer(powerMonitor, POWER_UPDATE_PERIOD, service);

			Fpga fpga;
			InventoryMonitor invMonitor(fpga, baseConn);
			PeriodicTimer<InventoryMonitor> invTimer(invMonitor, INVENTORY_UPDATE_PERIOD, service);

			//RangeFinderMonitor rfMonitor(fpga, wctrl);
			//PeriodicTimer<RangeFinderMonitor> rfTimer(rfMonitor, RANGEFINDER_UPDATE_PERIOD, service);

			// Start vision system.
			VisionCommImpl visionComm(baseConn, wctrl, arduino);
			Vision vision(visionComm, config.lidarComPort);
			
			// Add dummy work to prevent service.run() from exiting immediately.
			boost::asio::io_service::work work(service);
			service.run();
		} catch (const std::runtime_error& e) {
			Log::error("main: runtime_error: %s", e.what());
			throw;
		} catch (...) {
			Log::error("main: Unknown error");
			throw;
		}
		Log::info("Restarting connection");
	}
}

