#pragma once 

#include <stdafx.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//#include <iostream>
//#include <algorithm>

#include "rplidar.h"


class RPLidarController{

public:
	static void startLidar(std::string comport, std::string baudrate);
	static void test();
};