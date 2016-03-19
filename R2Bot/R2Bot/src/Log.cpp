#include "stdafx.h"
#include "Log.h"
#include <ctime>
#include <fstream>

const char LOGFILE[] = "log.txt";
const char* LABELS[] = { "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR" };

void print(const std::string& msg, int level) {
	// To reduce clutter, only tag warnings and errors
	if (level >= Log::LEVEL_WARN)
		std::cout << LABELS[level] << ": ";
	std::cout << msg << std::endl;
}

void log(const std::string& msg, int level) {
	if (level >= Log::level) {
		// Skip logging if msg is a duplicate of the previous msg.
		// This helps alleviate log bloat
		static std::string lastMsg;
		if (msg == lastMsg) { return; }
		lastMsg = msg;

		// Generate timestamp.
		std::time_t now = std::time(NULL);
		std::tm ptm;
		localtime_s(&ptm, &now);
		char timestamp[32];
		std::strftime(timestamp, 32, "[%H:%M:%S ", &ptm);

		// Write to log file.
		std::ofstream f(LOGFILE, std::ios::app);
		f << timestamp << LABELS[level] << "] " << msg << std::endl;
	}
}

void logAndPrint(const char* fmt, va_list args, int level) {
	char buf[250];
	vsprintf_s(buf, fmt, args);
	log(buf, level);
	print(buf, level);
}

int Log::level = Log::LEVEL_WARN;

// All log functions look similar, so we define them using a macro
#define LOGFUNC(name, level) \
	void Log:: ## name ## (const char* fmt, ...) { \
		va_list args; \
		va_start(args, fmt); \
		logAndPrint(fmt, args, LEVEL_ ## level); \
		va_end(args); \
	}

LOGFUNC(error, ERROR)
LOGFUNC(warn, WARN)
LOGFUNC(info, INFO)
LOGFUNC(debug, DEBUG)
LOGFUNC(trace, TRACE)