#include "StdAfx.h"
#include "BaseConnection.h"
#include "BaseToBotImpl.h"
#include <memory>

namespace R2D2 {

	BaseConnection::BaseConnection(BaseToBotImpl* baseToBot, int botPort, int basePort) {
		Log::trace("Starting server");
		this->baseToBot = baseToBot;
		this->basePort = basePort;
		this->communicator = CommPtr(Ice::initialize().get(),
			[](Ice::Communicator* c) { c->destroy(); });
		std::stringstream ss;
		ss << "tcp -p " << botPort;
		Ice::ObjectAdapterPtr adapter = communicator->
			createObjectAdapterWithEndpoints("BaseToBotImplAdapter", ss.str());
		adapter->add(baseToBot, communicator->stringToIdentity("BaseToBotImpl"));
		adapter->activate();
	}

	void BaseConnection::updateEncoderAndIMU(const BotPosition& enc, const BotPosition& imu) {
		updateConnections();
		for (auto& proxy : baseProxies) {
			// We don't care about exceptions, so we disregard the callback.
			proxy->begin_updateBotPosition(enc, imu);
		}
	}
	
	void BaseConnection::updateObstacleGrid(const ObstacleGrid& grid) {
		updateConnections();
		for (auto& proxy : baseProxies) {
			proxy->begin_updateObstacleGrid(grid);
		}
	}
	
	void BaseConnection::updatePowerStats(float batteryPercent, float voltage,
			float regulatedCurrent, float unregulatedCurrent) {
		updateConnections();
		for (auto& proxy : baseProxies) {
			proxy->begin_updatePowerStats(
				batteryPercent, voltage, regulatedCurrent, unregulatedCurrent);
		}
	}

	void BaseConnection::updateInventory(int inventory) {
		updateConnections();
		for (auto& proxy : baseProxies) {
			proxy->begin_updateInventory(inventory);
		}
	}

	void BaseConnection::stopAndGo(bool stopped){
		updateConnections();
		for (auto& proxy : baseProxies) {
			proxy->begin_stopAndGo(stopped);
		}
	}

	void BaseConnection::updateConnections() {
		std::unique_lock<std::mutex> guard(this->lock);
		auto hosts = baseToBot->getConnectedHosts();
		for (unsigned int i = baseProxies.size(); i < hosts.size(); i++) {
			std::stringstream ss;
			ss << "BotToBaseImpl:tcp -h " << hosts[i] << " -p " << basePort;
			Ice::ObjectPrx base = communicator->stringToProxy(ss.str());
			Log::info("Connecting to host: %s", hosts[i].c_str());
			baseProxies.push_back(BotToBasePrx::checkedCast(base)->ice_oneway());
			Log::info("Successfully connected");
		}
	}

}