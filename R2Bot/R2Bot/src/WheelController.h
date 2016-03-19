#pragma once

#include "MotorController.h"
#include "R2Comm.h"
#include "VisionComm.h"

namespace R2D2 {

	class WheelController : private MotorController {
	public:
		WheelController(const std::string& serialPort);
		virtual ~WheelController();
		void moveDist(int lDist, int rDist);
		void moveSpeed(int lSpeed, int rSpeed);
		void moveDistAtSpeed(int lDist, int lSpeed, int rDist, int rSpeed);
		void setEnabled(bool enable);
		std::pair<int,int> getDist();
		std::pair<int,int> getSpeed();
		std::pair<int,int> getPosition();
		WheelCommand getLastCommand();

	private:
		std::mutex cmdLock;
		WheelCommand lastCommand;
		bool isEnabled;
	};

}