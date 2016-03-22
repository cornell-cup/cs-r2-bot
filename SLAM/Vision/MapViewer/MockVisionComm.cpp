#include "stdafx.h"
#include "MockVisionComm.h"

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

MockVisionComm::MockVisionComm(R2D2::BotPosition& pos) : botPos(pos) { }

void MockVisionComm::updateObstacles(Ice::Long timestamp,
	const R2D2::BotPosition& pos, const R2D2::ObstacleList& obs) {
}

void MockVisionComm::updateObstacleGrid(const R2D2::ObstacleGrid& grid) {
	printf("\n\n");
	for (int i = 0; i < GRID_HEIGHT; i++) {
		Ice::Long row = grid[i];
		for (int j = 0; j < GRID_WIDTH; j++) {
			if ((int) (botPos.x + 0.5) == j && (int) (botPos.y + 0.5) == i) {
				putc('X', stdout);
			} else if ((row & (1ULL << j)) != 0) {
				putc('#', stdout);
			} else {
				putc('.', stdout);
			}
		}
		printf("\n");
	}
}

void MockVisionComm::updateBotPosition(const R2D2::BotPosition& pos) {
	botPos = pos;
}

R2D2::BotPosition& MockVisionComm::getBotPosition() {
	return botPos;
}