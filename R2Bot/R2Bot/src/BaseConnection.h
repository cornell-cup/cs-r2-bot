#pragma once

#include "R2Comm.h"
#include "BaseToBotImpl.h"
#include <string>
#include <functional>
#include <mutex>

namespace R2D2 {

	/// <summary>
	/// Maintains an ICE connection to Base.
	/// </summary>
	class BaseConnection {
	public:
		BaseConnection(BaseToBotImpl* baseToBot, int botPort, int basePort);

		void updateEncoderAndIMU(const BotPosition& enc, const BotPosition& imu);
		void updateObstacleGrid(const ObstacleGrid& grid);
		void updatePowerStats(float batteryPercent, float voltage,
				float regulatedCurrent, float unregulatedCurrent);
		void updateInventory(int inventory);
		void stopAndGo(bool stopped);

	private:
		void updateConnections();

		typedef std::unique_ptr<Ice::Communicator, std::function<void(Ice::Communicator*)>> CommPtr;

		int basePort;
		CommPtr communicator;
		BaseToBotImpl* baseToBot;
		std::vector<BotToBasePrx> baseProxies;
		std::mutex lock;
	};

}