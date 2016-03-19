#pragma once

#include "R2Comm.h"
#include "VisionComm.h"
#include "BaseConnection.h"
#include "Arduino.h"

namespace R2D2 {

	/// <summary>Implementation of Vision communication interface.</summary>
	class VisionCommImpl : public VisionComm {
	public:
		VisionCommImpl(BaseConnection& base, WheelController& wctrl, Arduino& ard);
		virtual void updateObstacleGrid(const ObstacleGrid& grid);
		virtual void updateBotPosition(const BotPosition& enc, const BotPosition& imu);
		virtual std::pair<int, int> getEncoderPositions();
		virtual WheelCommand getlastWheelCommand();
		virtual ImuData getImuData();
		virtual void stopAndGo(bool stopped);
		//virtual double getHeadRotationAngle(); //need to be implemented

		BaseConnection& baseConn;
		WheelController& wheelCtrl;
		Arduino& arduino;
	};
	

	/// <summary>Example of how to send vision data</summary>
	void runVision(VisionComm& comm, const std::string& lidarPort);
	
	void runLidar(VisionComm& comm, const std::string& lidarPort);
}