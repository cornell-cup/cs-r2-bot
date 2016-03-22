#pragma once
#include <stdio.h>
#include "BotComm/R2Comm.h"
#include <opencv2/opencv.hpp>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

class Kinect {
public:
	Kinect(R2D2::BotPosition& pos);
	Kinect();
	bool dataUsed;
	bool initKinect();
	int initBarcode();
	int testingInit(int argc, char* argv[]);
	USHORT getDepth(int x, int y);
	void scanForBarcode(cv::Mat img);
	void updateColorImage();
	DWORD WINAPI rgbFunc(LPVOID pParam);
	DWORD WINAPI depthFunc(LPVOID pParam);
	void startThreads();
	void processKey();
	void updateDepthImage();
private:
	R2D2::BotPosition& botPos;
	
};