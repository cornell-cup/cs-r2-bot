#pragma once

#include "MotorController.h"

namespace R2D2 {

	class HeadController  {
	public:
		HeadController(const std::string& headSerial, const std::string& flapSerial);
		void moveHead(int angle);
		void moveFlap(bool open);

	private:
		MotorController head;
		MotorController flap;
	};

}