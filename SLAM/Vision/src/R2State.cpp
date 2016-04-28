#include "stdafx.h"
#include "R2State.h"

R2State::R2State(double inPosX, double inPosY, double inRot,
	std::vector<Obstacle> inObstacles, bool inHighway, std::vector<double> inUltrasound, double inDepthDiff)
	: positionX(inPosX), positionY(inPosY), rotation(inRot), 
		obstacles(inObstacles), highway(inHighway), ultrasound(inUltrasound), depthDiff(inDepthDiff) {

}

R2State::~R2State() {

}
