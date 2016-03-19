#pragma once
#include <string>
#include "Ice/Ice.h"
#include "Logging.h"

class Client
{
public:
	Client(void);
	~Client(void);
	//Logging::RemoteLogPrx createClient(std::string, std::string, std::string);
	void static setRemoteLogPrx(Logging::RemoteLogPrx);
	void static logMessage(std::string, std::string, std::string);
	void static createClient(std::string, std::string, std::string);
	static Logging::RemoteLogPrx logStation;
};

