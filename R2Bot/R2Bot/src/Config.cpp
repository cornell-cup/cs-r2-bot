#include "stdafx.h"
#include "Config.h"
#include <boost/program_options.hpp>

namespace R2D2 {

	const char VERBOSE[] = "Log.Verbose";
	const char REMOTE_LOG_IP[] = "Log.RemoteLogIp";
	const char BOT_PORT[] = "Network.BotPort";
	const char BASE_PORT[] = "Network.BasePort";
	const char CONN_TIMEOUT[] = "Network.ConnectionTimeout";
	const char ARDUINO_COM_PORT[] = "Devices.ArduinoComPort";
	const char WHEELS_COM_PORT[] = "Devices.WheelsComPort";
	const char HEAD_COM_PORT[] = "Devices.HeadComPort";
	const char FLAP_COM_PORT[] = "Devices.FlapComPort";
	const char LIDAR_COM_PORT[] = "Devices.LidarComPort";

	const Config DEFAULT_CONFIG = {
		Log::LEVEL_WARN, // verbose
		std::string("localhost"),
		10000, // botPort
		10001, // basePort
		1000, // connectionTimeout
//		std::string("FAKE_SERIAL"), // arduinoComPort
		std::string("FAKE_SERIAL"), // wheelsComPort
		std::string("FAKE_SERIAL"), // headComPort
		std::string("FAKE_SERIAL"), // flapComPort
//		std::string("FAKE_SERIAL"), // lidarComPort
	};

	Config Config::fromFile(const char* filename) {
		using namespace boost::program_options;

		// Define valid options and their types.
		options_description desc;
		desc.add_options()
			(VERBOSE, value<int>())
			(REMOTE_LOG_IP, value<std::string>())
			(BOT_PORT, value<int>())
			(BASE_PORT, value<int>())
			(CONN_TIMEOUT, value<int>())
			//			(ARDUINO_COM_PORT, value<std::string>())
			(WHEELS_COM_PORT, value<std::string>())
			(HEAD_COM_PORT, value<std::string>())
			(FLAP_COM_PORT, value<std::string>());
//			(LIDAR_COM_PORT, value<std::string>());
	
		// Parse the config file
		try {
			variables_map vm;
			store(parse_config_file<char>(filename, desc), vm);
			notify(vm);

			Config cfg;
			cfg.verbose = vm[VERBOSE].as<int>();
			cfg.remoteLogIp = vm[REMOTE_LOG_IP].as<std::string>();
			cfg.basePort = vm[BOT_PORT].as<int>();
			cfg.botPort = vm[BASE_PORT].as<int>();
			cfg.connTimeout = vm[CONN_TIMEOUT].as<int>();
//			cfg.arduinoComPort = vm[ARDUINO_COM_PORT].as<std::string>();
			cfg.wheelsComPort = vm[WHEELS_COM_PORT].as<std::string>();
			cfg.flapComPort = vm[FLAP_COM_PORT].as<std::string>();
			cfg.headComPort = vm[HEAD_COM_PORT].as<std::string>();
//			cfg.lidarComPort = vm[LIDAR_COM_PORT].as<std::string>();
			return cfg;
		} catch (std::exception& e) {
			Log::error("Failed to read config file: %s", e.what());
			Log::info("Using default config values");
			return DEFAULT_CONFIG;
		}
	}

}