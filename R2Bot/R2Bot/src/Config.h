#pragma once
#include <string>

namespace R2D2 {

	struct Config {
		int verbose;
		std::string remoteLogIp;
		int basePort;
		int botPort;
		int connTimeout;
		std::string arduinoComPort;
		std::string wheelsComPort;
		std::string headComPort;
		std::string flapComPort;
		std::string lidarComPort;

		static Config fromFile(const char* filename);
	};

}