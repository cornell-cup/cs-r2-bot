#pragma once

#include "Obstacle.h"

class R2State {
protected:
	// Private/protected variables here
public:
	/**
	 * Create a new R2 state
	 * 
	 * @param inPosX	The X coordinate of R2
	 * @param inPosY	The Y coordinate of R2
	 * @param inRot		The rotation of R2 (in radians)
	 * @param inObstacles	A vector of obstacles in the world
	 * @param inHighway		A boolean whether or not a highway has been detected
	 * @param inUltrasound	A vector of booleans corresponding to each ultrasound sensor
	 * @param inDepthDiff	The depth difference between consecutive Kinect frames
	 */
	R2State(double inPosX = 0.0, double inPosY = 0.0, double inRot = 0.0,
		std::vector<Obstacle> inObstacles = std::vector<Obstacle>(), bool inHighway = false,
		std::vector<double> inUltrasound = std::vector<double>(), double inDepthDiff = 0.0);

	/**
	 * Destructors
	 */
	~R2State();

	// TODO documentation
	double positionX;
	double positionY;
	double rotation;
	std::vector<Obstacle> obstacles;
	bool highway;
	std::vector<double> ultrasound;
	double depthDiff;
};
