// LogClient.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "Client.h"
#include "stdafx.h"
#include "Ice/Ice.h"
#include "Logging.h"
#include <time.h>

//using namespace Client;
using namespace Logging;

int main()
{
	//Client myClient;
	//RemoteLogPrx logPrx = myClient.createClient("localhost", "10003", "Ping");
	//myClient.setRemoteLogPrx(logPrx);
	Client::createClient("localhost", "10003", "Ping");

	time_t rawtime; 
	struct tm * timeinfo;
	struct tm * newtime;
	time(&rawtime);

	timeinfo = localtime(&rawtime);
	//std::string time1 = asctime(timeinfo);
	
	/*
	while (true) {
		time(&rawtime);
		newtime = localtime(&rawtime);
		std::string time2 = asctime(newtime);

		//std::cout << (*timeinfo).tm_sec << std::endl;
		std::cout << (*newtime).tm_sec << " " << (*timeinfo).tm_sec << std::endl;
		if ((*timeinfo).tm_sec != (*newtime).tm_sec){
			std::cout << "entered if" << std::endl;
			myClient.logMessage("mycomputer", time2, "log message");
			timeinfo = localtime(&rawtime);
		}
	}*/

	int counter = 1;
	while (true){	
		//newtime = localtime(&rawtime);
		std::string time = asctime(timeinfo);
		//std::string msg = "log message " + std::to_string(counter);
		Client::logStation->log("mycomputer", time, "log message " + std::to_string (counter));
		counter++;
	}
	


	return 0;
}

/*
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/
