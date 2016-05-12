#include "stdafx.h"
#include "Vision.h"
#include "PeriodicTimer.h"
#include <thread>
#include <math.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "easywsclient.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

#include "LidarSensor.h"
#include "UltrasoundSensor.h"
#include "NUCSensor.h"

#define M_PI 3.1415926536
const float PI2 = 2.f * 3.1415926f;
const float FEET_PER_CM = 0.0328084f;
const float FEET_PER_METER = 3.28084f;

// Spacing between wheels in cm, measured from center of one wheel to center of the other.
const float WHEEL_SPACING = 52.5;
const float HALF_WHEEL_SPACING = WHEEL_SPACING / 2;

Vision::Vision(R2D2::VisionComm& visionComm, const std::string& lidarComPort)
		: grid(GRID_HEIGHT), visionComm(visionComm), lPos(0), rPos(0),
            accKF(0.0, 1.0, 0.001, 0.1), velKF(0.0, 0.1, 0.001, 0.01), rotKF(0.0, 0.0, 0.001, 0.01) {

	botPosition.x = botPosition.y = 30.5;
	botPosition.theta = 0;
    
	// Initialize winsock
	WSADATA wsaData;
	int wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// NUC Data
	// TODO Receive this from the configuration file
	Sensor * nuc = new NUCSensor("0.0.0.0", 11000);
	sensors.push_back(nuc);

	// RP Lidar
	Sensor * lidar = new LidarSensor(lidarComPort);
	sensors.push_back(lidar);

	// Ultrasound
	std::vector<std::string> locations;
	locations.push_back("Front");
	locations.push_back("Cliff");
	locations.push_back("Left shoulder");
	locations.push_back("Right shoulder");
	locations.push_back("Back");
	//Sensor * ultrasound = new UltrasoundSensor("\\\\.\\com11", locations);
	//sensors.push_back(ultrasound);

	std::thread([this] {
		std::function<void()> updateBotPositionFn = [this] { updateBotPosition(); };
		PeriodicTimer<std::function<void()>> periodicEncoder(updateBotPositionFn, 50, service);

		std::function<void()> updateOccupancyGridFn = [this] { updateOccupancyGrid(); };
		PeriodicTimer<std::function<void()>> periodicOccupancy(updateOccupancyGridFn, 200, service);
        
        std::function<void()> updateDataBagFn = [this] { updateDataBag(); };
        PeriodicTimer<std::function<void()>> periodicData(updateDataBagFn, 100, service);
         
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
    // Clear initial grid
    for (int i = 0; i < GRID_HEIGHT; i++) {
        grid[i] = (Ice::Long) 0;
    }
    
	// Lidar obstacles
	LidarData * ld = sdata->lidar;
	if (ld != NULL && ld->hasData) {
		int l = ld->angles.size();
		for (int i = 0; i < l; i++) {
			double angle = ld->angles[i] / 180.0 * M_PI + botPosition.theta;
			double distance = FEET_PER_METER * ld->distances[i];
			int px = int(distance * cos(angle * PI / 180.0) + botPosition.x);
			int py = int(distance * sin(angle * PI / 180.0) + botPosition.y);
            if (0 <= py && py < GRID_HEIGHT && 0 <= px && px < GRID_WIDTH) {
                grid[i] |= 1ULL << px;
            }
		}
	}

	// NUC / Kinect obstacles
	KinectData * kd = sdata->kinect;
	if (kd != NULL && kd->hasData) {
		// TODO
	}
    
	visionComm.updateObstacleGrid(grid);
}

void Vision::updateBotPosition() {
	printf("Updating bot position!\n");
	visionComm.updateBotPosition(getEncoderPosition(), getImuPosition());
}

void Vision::updateDataBag() {
    if (sdata != NULL) {
        delete sdata;
    }
    for (auto sensor = sensors.begin(); sensor != sensors.end(); sensor++) {
		(*sensor)->getData(sdata);
	}
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
    pos.x = botPosition.x;
    pos.y = botPosition.y;
	pos.theta = botPosition.theta;
    
    IMUData * id = sdata->imu;
    if (id != NULL && id->hasData) {
        double accMag = sqrt(id->accelX * id->accelX + id->accelY * id->accelY);
        double nextAcc = accKF.update(accMag);
        double nextVel = velKF.update(nextAcc * id->dt);
		pos.theta = (float) (id->gyroX);
		pos.x += (float) (nextVel * id->dt * cos(pos.theta));
		pos.y += (float) (nextVel * id->dt * sin(pos.theta));
    }
    
    botPosition.x = FEET_PER_METER * pos.x;
    botPosition.y = FEET_PER_METER * pos.y;
	botPosition.theta = pos.theta;
	return pos;
}
