// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define BOOST_ALL_DYN_LINK

#include "targetver.h"

#include <thread>
#include <boost/asio/io_service.hpp>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

#define PI 3.141592654

#include <string>
#include <vector>

#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
