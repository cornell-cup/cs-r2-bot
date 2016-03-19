#pragma once

#include "R2Comm.h"

#define GRID_HEIGHT 60
#define GRID_WIDTH  60

namespace R2D2 {

	struct WheelCommand {
		int lSpeed;
		int rSpeed;
		enum WheelCommandType { STRAIGHT, TURN } type;
	};

	struct ImuData {
		float yaw;
		float pitch;
		float roll;
		float ax; //acceleration in x-axis
		float ay;
		float az;
	};
	
	// Interface for Vision communication.
	class VisionComm {
	public:
		virtual void updateObstacleGrid(const ObstacleGrid& grid) = 0;
		virtual void updateBotPosition(const BotPosition& enc, const BotPosition& imu) = 0;
		virtual std::pair<int, int> getEncoderPositions() = 0;
		virtual WheelCommand getlastWheelCommand() = 0;
		virtual ImuData getImuData() = 0;

		// Tells base whether R2 is stopped/scanning for a barcode or moving
		// True if stopped, otherwise false
		virtual void stopAndGo(bool stopped) = 0;

		//Involving arduino and IMU, not tested yet
		//virtual ImuData getImuData() = 0;
	};

}