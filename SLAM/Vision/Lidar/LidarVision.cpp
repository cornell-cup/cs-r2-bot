#include "stdafx.h"
#include "LidarVision.h"

const double METERS_PER_LIDAR_UNIT = 1.0 / 1000.0;
const double FEET_PER_METER = 3.28084;
const double SCALE = FEET_PER_METER * METERS_PER_LIDAR_UNIT;
// Offset due to Lidar being installed at an angle, rather than facing forward
const double ANGLE_OFFSET = 0;

double degreeToRad(float degrees) {
	return degrees / 180.0 * M_PI;
}

int iround(double v) {
	return static_cast<int>(v + .5);
}

LidarVision::LidarVision(const char* comPort, R2D2::BotPosition& pos) : lidar(comPort), botPos(pos) {
	std::memset(grid, 0, sizeof(grid));

	std::thread([this]() {
		while (true) {
			// Take snapshot of botPos state, which is externally modifiable.
			R2D2::BotPosition pos = botPos;
			std::vector<PolarPoint> points = lidar.readData();
			for (auto& pt : points) {
				double angle = pos.theta - degreeToRad(pt.angle) + ANGLE_OFFSET;
				double depth = SCALE * pt.depth;
				double x = pos.x + depth * cos(angle);
				double y = pos.y + depth * sin(angle);
				// We may need to shift obstacles to prevent them from appearing on top of R2
				if (iround(x) == iround(pos.x) && iround(y) == iround(pos.y)) {
					x = pos.x + (depth+1) * cos(angle);
					y = pos.y + (depth+1) * sin(angle);
				}
				int row = iround(y);
				int col = iround(x);
				if ((0 <= row && row <= GRID_HEIGHT) && (0 <= col && col <= GRID_WIDTH)) {
					grid[row][col] += 1;
				}
			}
		}
	}).detach();
}