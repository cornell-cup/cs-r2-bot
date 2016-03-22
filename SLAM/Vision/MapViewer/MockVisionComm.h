#pragma once

#include "BotComm/VisionComm.h"

class MockVisionComm : public R2D2::VisionComm {
public:
	MockVisionComm(R2D2::BotPosition& pos);

	virtual void updateObstacles(Ice::Long timestamp, const R2D2::BotPosition& pos,
		const R2D2::ObstacleList& obs);

	virtual void updateObstacleGrid(const R2D2::ObstacleGrid& grid);

	virtual void updateBotPosition(const R2D2::BotPosition& pos);

	virtual R2D2::BotPosition& getBotPosition();

private:
	R2D2::BotPosition& botPos;
};