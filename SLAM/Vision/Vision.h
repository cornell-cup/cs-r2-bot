#pragma once

#include "BotComm/R2Comm.h"
#include "BotComm/VisionComm.h"
#include "Kinect/Kinect.h"
#include "Lidar/LidarVision.h"
#include <boost/asio/io_service.hpp>
#include <chrono>

class Vision {
public:
	Vision(R2D2::VisionComm& comm, const std::string& lidarComPort);
	void updateOccupancyGrid();
	void updateBotPosition();
	R2D2::BotPosition getEncoderPosition();
	R2D2::BotPosition getImuPosition();
	//double getHeadRotationAngle(); //need to be implemented

	//Kinect kinect;
	LidarVision lidar;
	R2D2::ObstacleGrid grid;
	R2D2::VisionComm& visionComm;
	boost::asio::io_service service;

	R2D2::BotPosition botPosition;
	int lPos;
	int rPos;

	std::chrono::high_resolution_clock::time_point lastImuUpdate;
	float ax_hat = 0;
	float ay_hat = 0;
	float vx_hat = 0;
	float vy_hat = 0;
	
};