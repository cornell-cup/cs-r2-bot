#pragma once

#include "Util.h"
#include "Serial.h"
#include <string>
#include <regex>

namespace R2D2 {

	class MotorController {
	public:
		MotorController(const std::string& serialPort);
		virtual ~MotorController();
		int readValue(const char* command, const std::regex& parser);
		void send(const char* fmt, ...);
		std::string recv();

	private:
		Serial serial;
	};

}