#pragma once

#include "BotComm/R2Comm.h"
#include "BotComm/VisionComm.h"
#include <boost/asio/io_service.hpp>
#include <chrono>
#include <vector>

#include "Sensor.h"
#include "SensorDataBag.h"
#include "ConstantKalmanFilter.h"

class Vision {
public:
	Vision(R2D2::VisionComm& comm, const std::string& lidarComPort);
    
	void updateOccupancyGrid();
	void updateBotPosition();
    void updateDataBag();
    
	R2D2::BotPosition getEncoderPosition();
	R2D2::BotPosition getImuPosition();
	
    // Communication and running service
	R2D2::VisionComm& visionComm;
	boost::asio::io_service service;
    
    // List of sensors and sensor data
    std::vector<Sensor *> sensors;
    SensorDataBag * sdata;
	R2D2::ObstacleGrid grid;
	R2D2::BotPosition botPosition;
    
	int lPos;
	int rPos;
    
    // KFs for acceleration, velocity, and theta
    ConstantKalmanFilter accKF, velKF, rotKF;
};
