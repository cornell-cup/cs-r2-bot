#include "StdAfx.h"
#include "Client.h"

#include "stdafx.h"
#include "Ice/Ice.h"
#include "Logging.h"
#include <string>


using namespace Logging;

RemoteLogPrx Client::logStation = NULL;

void Client::setRemoteLogPrx (RemoteLogPrx p){
	logStation = p;
}

/*
void Client::logMessage (std::string name, std::string time, std::string msg){
	logStation->log(name, time, msg);
	
}*/

void Client::createClient (std::string ip, std::string port, std::string proxyname){
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize();
		std::string str = proxyname + ":tcp" + " -h " + ip + " -p " + port;
		RemoteLogPrx logS = RemoteLogPrx::checkedCast(ic->stringToProxy(str));
		std::cout << "Connection Successful" << std::endl;
		logStation = logS;
		//return logS;

		/*
		if (ic)
			ic->destroy();
		*/
	}
	catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}

};

Client::Client(void)
{	
}


Client::~Client(void)
{
}
