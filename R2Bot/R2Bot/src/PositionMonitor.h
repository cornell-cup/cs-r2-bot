#pragma once

#include "BaseConnection.h"

namespace R2D2 {

	class PositionMonitor {
	public:
		PositionMonitor(WheelController& wctrl, BaseConnection& base,
			BotPosition& pos, WheelCommand& lastCommand);
		void operator()();
		const BotPosition& getPosition();

	private:
		WheelController& wheelCtrl;
		BaseConnection& baseConn;
		WheelCommand& lastCommand;
		int _lPos;
		int _rPos;
		BotPosition& _pos;
	};

}