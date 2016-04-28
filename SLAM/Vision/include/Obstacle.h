#pragma once

class Obstacle {
protected:
public:
	/**
	 * Create a new obstacle.
	 * 
	 * @param inPosX	The X coordinate of the obstacle
	 * @param inPosY	The Y coordinate of the obstacle
	 * @param inRadius	The radius of the obstacle
	 * @param inHeight	The height of the obstacle
	 */
	Obstacle::Obstacle(double inPosX = 0.0, double inPosY = 0.0, double inRadius = 0.0, double inHeight = 0.0);

	/**
	 * Destructor
	 */
	Obstacle::~Obstacle();

	/**
	 * Checks if another object is within this object.
	 * 
	 * @param inPosX	The X coordinate of the object in question
	 * @param inPosY	The Y coordinate of the object in question
	 * @param inRadius	The radius of the object in question
	 */
	bool within(double inPosX, double inPosY, double inRadius);

	/**
	 * X coordinate of the obstacle
	 */
	double positionX;
	/**
	 * Y coordinate of the obstacle
	 */
	double positionY;
	/**
	 * Radius of the obstacle
	 */
	double radius;
	/**
	 * Height of the obstacle
	 */
	double height;
};
