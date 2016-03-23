#include "stdafx.h"
#include "HeadController.h"

namespace R2D2 {
	
	const int FLAP_OPEN_COMMAND = 0;
	const int FLAP_CLOSE_COMMAND = 0;
	

	HeadController::HeadController(const std::string& headSerial, const std::string& flapSerial)
			: head(headSerial.c_str()), flap(flapSerial.c_str()) {
		head.send("1,start");
		head.send("1,home");
		flap.send("1,start");
		flap.send("1,home");
	}

	void HeadController::moveHead(int angle) {
		head.send("1,p%d", angle);
	}

	void HeadController::moveFlap(bool open) {
		flap.send("1,p%d", open ? FLAP_OPEN_COMMAND : FLAP_CLOSE_COMMAND);
	}
}