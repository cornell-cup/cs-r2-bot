#include "stdafx.h"
#include "VisionCommImpl.h"
#include "BaseConnection.h"
#include "Util.h"
#include <boost/asio.hpp>
#include <cstdlib>

namespace R2D2 {

	const float LIDAR_DIST_FACTOR = 32.0f / 2000.0f;

	VisionCommImpl::VisionCommImpl(BaseConnection& base, WheelController& wctrl, Arduino& ard)
		: baseConn(base), wheelCtrl(wctrl), arduino(ard) { }

	void VisionCommImpl::updateObstacleGrid(const ObstacleGrid& grid) {
		try {
			baseConn.updateObstacleGrid(grid);
		} catch (...) {
			Log::warn("Failure while sending obstacle grid");
		}
	}

	void VisionCommImpl::updateBotPosition(const BotPosition& enc, const BotPosition& imu) {
		try {
			baseConn.updateEncoderAndIMU(enc, imu);
		}
		catch (...) {
			Log::warn("Failure while sending bot position");
		}
	}

	std::pair<int, int> VisionCommImpl::getEncoderPositions(){
		return wheelCtrl.getPosition();
	}

	ImuData VisionCommImpl::getImuData() {
		return arduino.getImuData();
	}

	WheelCommand VisionCommImpl::getlastWheelCommand() {
		return wheelCtrl.getLastCommand();
	}

	void VisionCommImpl::stopAndGo(bool stopped){
		try {
			baseConn.stopAndGo(stopped);
		}
		catch (...){
			Log::warn("Failure while sending if bot is stopped");
		}
	}
}