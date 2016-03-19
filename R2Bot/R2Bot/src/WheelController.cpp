#include "stdafx.h"
#include "WheelController.h"
#include "Util.h"
#include <utility>
#include <varargs.h>

namespace R2D2 {

	// Regex for extracting position from Kangaroo output.
	const std::regex GET_POSITION_REGEX("[1-2],[pP](-?[0-9]+)\r\n");
	// Regex for extracting speed from Kangaroo output.
	const std::regex GET_SPEED_REGEX("[1-2],[sS](-?[0-9]+)\r\n");
	
	WheelController::WheelController(const std::string& serialPort)
			: MotorController(serialPort), isEnabled(true) {
		lastCommand.lSpeed = 0;
		lastCommand.rSpeed = 0;
		lastCommand.type = WheelCommand::STRAIGHT;

		send("1,start");
		send("2,start");
		send("1,home");
		send("2,home");
	}

	WheelController::~WheelController() {
		try { moveSpeed(0, 0); } catch (...) { }
	}

	void WheelController::moveDist(int lDist, int rDist) {
		std::unique_lock<std::mutex> guard(cmdLock);
		send("1,pi%d", lDist);
		send("2,pi%d", rDist);
	}
	
	void WheelController::moveSpeed(int lSpeed, int rSpeed) {
		std::unique_lock<std::mutex> guard(cmdLock);
		if (!isEnabled) { return; }
		if (lSpeed != lastCommand.lSpeed || rSpeed != lastCommand.rSpeed) {
			send("1,s%d", rSpeed);
			send("2,s%d", lSpeed);
			lastCommand.lSpeed = lSpeed;
			lastCommand.rSpeed = rSpeed;
			// We don't change the command type when stopping. This helps the PositionMonitor
			// stay accurate at the tail-end of movements, when switching speeds.
			if (lSpeed != 0 && rSpeed != 0) {
				lastCommand.type = lSpeed == rSpeed ? WheelCommand::STRAIGHT : WheelCommand::TURN;
			}
		}
	}

	void WheelController::moveDistAtSpeed(int lDist, int lSpeed, int rDist, int rSpeed) {
		std::unique_lock<std::mutex> guard(cmdLock);
		send("1,pi%ds%d", lDist, lSpeed);
		send("2,pi%ds%d", rDist, rSpeed);
	}

	void WheelController::setEnabled(bool value) {
		std::unique_lock<std::mutex> guard(cmdLock);
		if (isEnabled && !value) { moveSpeed(0, 0); }
		isEnabled = value;
	}

	std::pair<int,int> WheelController::getDist() {
		std::unique_lock<std::mutex> guard(cmdLock);
		return std::make_pair(
			readValue("1,getpi", GET_POSITION_REGEX),
			readValue("2,getpi", GET_POSITION_REGEX));
	}

	std::pair<int,int> WheelController::getSpeed() {
		std::unique_lock<std::mutex> guard(cmdLock);
		return std::make_pair(
			readValue("1,gets", GET_SPEED_REGEX),
			readValue("2,gets", GET_SPEED_REGEX));
	}

	std::pair<int,int> WheelController::getPosition() {
		std::unique_lock<std::mutex> guard(cmdLock);
		return std::make_pair(
			readValue("2,getp", GET_POSITION_REGEX),
			readValue("1,getp", GET_POSITION_REGEX));
	}

	WheelCommand WheelController::getLastCommand() {
		return lastCommand;
	}
}