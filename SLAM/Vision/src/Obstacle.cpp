#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(double inPosX, double inPosY, double inRadius, double inHeight)
	: positionX(inPosX), positionY(inPosY), radius(inRadius), height(inHeight) {

}

Obstacle::~Obstacle() {

}

bool Obstacle::within(double inPosX, double inPosY, double inRadius) {
	double dx = inPosX - positionX;
	double dy = inPosY - positionY;
	double dr = inRadius + radius;
	return dx * dx + dy * dy < dr * dr;
}
