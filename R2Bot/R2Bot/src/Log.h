#pragma once
#include <string>
#include "Ice/Ice.h"

/// <summary>A simple logging class.
/// Each logging function writes to the stdout.
/// They also conditionally writes to log.txt depending on `level`.
///</summary>
class Log {
public:

	static const int LEVEL_NONE  = 5;
	static const int LEVEL_ERROR = 4;
	static const int LEVEL_WARN  = 3;
	static const int LEVEL_INFO  = 2;
	static const int LEVEL_DEBUG = 1;
	static const int LEVEL_TRACE = 0;

	static int level;

	static void error(const char* fmt, ...);
	static void warn(const char* fmt, ...);
	static void info(const char* fmt, ...);
	static void debug(const char* fmt, ...);
	static void trace(const char* fmt, ...);
};