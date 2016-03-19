#include "stdafx.h"
#include "PositionMonitor.h"

namespace R2D2 {

	const float PI2 = static_cast<float>(2 * M_PI);
	const float FEET_PER_CM = 0.0328084f;

	// Normalize an angle to the range [0, 2pi)
	float constrainAngle(float radians) {
		float rem = fmod(radians, PI2);
		return rem < 0 ? rem + PI2 : rem;
	}

	// Spacing between wheels in cm, measured from center of one wheel to center of the other.
	const float WHEEL_SPACING = 52.5;
	const float HALF_WHEEL_SPACING = WHEEL_SPACING / 2;

	PositionMonitor::PositionMonitor(WheelController& wctrl, BaseConnection& base,
		BotPosition& pos, WheelCommand& lastCmd)
			: wheelCtrl(wctrl), baseConn(base), lastCommand(lastCmd), _lPos(0), _rPos(0), _pos(pos)
	{
	}
	
	void PositionMonitor::operator()() {
		std::pair<int,int> wheelPos = wheelCtrl.getPosition();
		int dL = wheelPos.first - _lPos;
		int dR = wheelPos.second - _rPos;

		if (lastCommand.type == WheelCommand::STRAIGHT) {
			float dist = FEET_PER_CM * (dR + dL) / 2.0f;
			_pos.x += dist * cos(_pos.theta);
			_pos.y += dist * sin(_pos.theta);
		} else {
			_pos.theta = constrainAngle(_pos.theta + (dL - dR) / WHEEL_SPACING);
		}

		_lPos = wheelPos.first;
		_rPos = wheelPos.second;

		Log::debug("Sending position (%f, %f)", _pos.x, _pos.y);
		//baseConn.updateEncoderPosition(_pos);
	}

}