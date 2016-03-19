// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#define _USE_MATH_DEFINES
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define BOOST_ASIO_NO_WIN32_LEAN_AND_MEAN
#define BOOST_ALL_DYN_LINK

#include <boost/asio.hpp>
#include "targetver.h"

#include "Log.h"

#include <boost/program_options.hpp>
//#include <boost/asio.hpp>

#include <Ice/Ice.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <regex>
#include <functional>
#include <exception>
#include <algorithm>
#include <mutex>
#include <condition_variable>

#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <varargs.h>

#include <windows.h>
#include <MMSystem.h>
#include <tchar.h>