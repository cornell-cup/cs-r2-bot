#include "stdafx.h"
#include "MotorController.h"


namespace R2D2 {
	
	const char* kangarooErrorMsg(std::string& err) {
		std::string e = err.substr(0, 2); // Strip off \r\n
		if (e == "E1") return "Not started";
		if (e == "E2") return "Not homed";
		if (e == "e2") return "Homing in progress";
		if (e == "E3") return "Control error, channel disabed";
		if (e == "E4") return "System is in the wrong mode";
		if (e == "E5") return "Readback command not recognized";
		if (e == "E6") return "Signal lost";
		return "Unknown error";
	}

	MotorController::MotorController(const std::string& serialPort)
		: serial(serialPort.c_str()) { }

	MotorController::~MotorController() { }
	
	int MotorController::readValue(const char* command, const std::regex& parser) {
		send(command);
		std::string reply = recv();
		std::smatch match;
		if (!std::regex_match(reply, match, parser))
			throw std::runtime_error(kangarooErrorMsg(reply));
		return std::stoi(match[1].str());
	}

	void MotorController::send(const char* fmt, ...) {
		char buf[128];
		va_list args;
		va_start(args, fmt);
		int len = vsprintf_s(buf, fmt, args);
		va_end(args);
		Log::trace("Motor send: %s", buf);
		serial.write(buf, len);
		serial.write("\r\n", 2);
	}

	std::string MotorController::recv() {
		char buf[1024];
		int len = 0;
		// Poll until the last character is \n
		while (!(len > 0 && buf[len-1] == '\n')) {
			len += serial.tryRead(buf+len, sizeof(buf)-len);
			buf[len] = 0;
		}
		Log::trace("Motor recv: %s", buf);
		return std::string(buf, len);
	}

}