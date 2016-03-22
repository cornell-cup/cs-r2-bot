#include "stdafx.h"
#include "Vision.h"
#include "MockVisionComm.h"

const std::string lidarComPort = "COM7";

int main() {
	R2D2::BotPosition botPos = { 30, 30, 3.14159265/4 };
	MockVisionComm comm(botPos);
	Vision v(comm, lidarComPort);

	while (true) {
		//botPos.theta += M_PI / 4;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}