#include "stdafx.h"
#include "Vision.h"
#include "PeriodicTimer.h"
#include <thread>
#include <math.h>

const float PI2 = 2 * 3.1415926;
const float FEET_PER_CM = 0.0328084f;

// Spacing between wheels in cm, measured from center of one wheel to center of the other.
const float WHEEL_SPACING = 52.5;
const float HALF_WHEEL_SPACING = WHEEL_SPACING / 2;

Vision::Vision(R2D2::VisionComm& visionComm, const std::string& lidarComPort)
		: /*kinect(botPosition),*/ lidar(lidarComPort.c_str(), botPosition), grid(GRID_HEIGHT),
		visionComm(visionComm), lPos(0), rPos(0) {

	botPosition.x = botPosition.y = 30.5;
	botPosition.theta = 0;

	std::thread([this] {
		std::function<void()> updateOccupancyGridFn = [this] { updateOccupancyGrid(); };
		PeriodicTimer<std::function<void()>> periodicOccupancy(updateOccupancyGridFn, 200, service);

		std::function<void()> updateBotPositionFn = [this] { updateBotPosition(); };
		PeriodicTimer<std::function<void()>> periodicEncoder(updateBotPositionFn, 2000, service);

		// Add dummy work to prevent service.run() from exiting immediately.
		boost::asio::io_service::work work(service);
		service.run();
	}).detach();
}

// Normalize an angle to the range [0, 2pi)
float constrainAngle(float radians) {
	float rem = fmod(radians, PI2);
	return rem < 0 ? rem + PI2 : rem;
}

void Vision::updateOccupancyGrid() {
	// Generate obstacle grid
	for (int i = 0; i < GRID_HEIGHT; i++) {
		Ice::Long row = 0;
		for (int j = 0; j < GRID_WIDTH; j++) {
			//int kValue = kinect.grid[i][j];
			//if (kValue > 0) { kinect.grid[i][j] = kValue - 250; }

			int lValue = lidar.grid[i][j];
			if (lValue > 1) { lidar.grid[i][j] = lValue - 1; }

			if (/*kValue > 25 ||*/ lValue > 1) {
				row |= 1ULL << j; // use ULL to ensure 64 bit shift
			}
		}
		grid[i] = row;
	}
	visionComm.updateObstacleGrid(grid);
}

void Vision::updateBotPosition() {
	printf("Updating bot position!\n");
	visionComm.updateBotPosition(getEncoderPosition(), getImuPosition());
}

R2D2::BotPosition Vision::getEncoderPosition() {
	printf("Getting encoder positions\n");
	std::pair<int, int> wheelPos = visionComm.getEncoderPositions();
	R2D2::WheelCommand lastCommand = visionComm.getlastWheelCommand();
	int dL = wheelPos.first - lPos;
	int dR = wheelPos.second - rPos;

	R2D2::BotPosition pos = { 0 };
	if (lastCommand.type == R2D2::WheelCommand::STRAIGHT) {
		float dist = FEET_PER_CM * (dR + dL) / 2.0f;
		pos.x = dist * cos(botPosition.theta);
		pos.y = dist * sin(botPosition.theta);
	} else {
		pos.theta = constrainAngle((dL - dR) / WHEEL_SPACING);
	}

	lPos = wheelPos.first;
	rPos = wheelPos.second;

	printf("Encoders: x %f y %f th %f\n", pos.x, pos.y, pos.theta);

	return pos;
}

R2D2::BotPosition Vision::getImuPosition() {
	printf("Getting IMU position\n");
	R2D2::BotPosition pos;
	R2D2::ImuData imu = visionComm.getImuData();
	auto now = std::chrono::high_resolution_clock::now();
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastImuUpdate).count();
	lastImuUpdate = now;
	printf("lastImuUpdate: %f, dt: %f\n", lastImuUpdate, dt);
	ax_hat = imu.ax;
	vx_hat += ax_hat * dt;
	pos.x = vx_hat * dt;


	ay_hat = imu.ay;
	vy_hat += ay_hat * dt;
	pos.y = vy_hat * dt;

	pos.theta = imu.yaw;

	printf("IMU: x %f y %f th %f\n", pos.x, pos.y, pos.theta);
	printf("Velocity x: %f, Velocity y: %f\n", vx_hat, vy_hat);

	return pos;
}
