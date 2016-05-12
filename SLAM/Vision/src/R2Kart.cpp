// R2Kart.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "easywsclient.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

#include <armadillo.hpp>

#include "R2State.h"
#include "Sensor.h"
#include "SensorData.h"
#include "SensorDataBag.h"
#include "KinectSensor.h"
#include "LidarSensor.h"
#include "PhoneIMUSensor.h"
#include "UltrasoundSensor.h"

using easywsclient::WebSocket;

/**
 * The connection to the GUI server.
 */
SOCKET connection = INVALID_SOCKET;

/**
* Initialize Winsock.
*/
void initializeWinsock() {
	// Variables
	WSADATA wsaData;
	int wsResult;

	// Startup winsock
	wsResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/**
 * Initialize a Winsock connection to localhost.
 */
void initializeSocketCommunication() {
	// Variables
	int wsResult;

	// Connection address info (loopback)
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	wsResult = getaddrinfo("localhost", "9000", &hints, &result);
	if (wsResult != 0) {
		WSACleanup();
		return;
	}

	// Try connections
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a socket
		connection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connection == INVALID_SOCKET) {
			WSACleanup();
			return;
		}

		// Connect to server
		wsResult = connect(connection, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (wsResult == SOCKET_ERROR) {
			// Error connecting
			closesocket(connection);
			connection = INVALID_SOCKET;
		}
		else {
			// All good, we can stop here
			break;
		}
	}

	freeaddrinfo(result);

	if (connection == INVALID_SOCKET) {
		WSACleanup();
	}
}

/**
 * Connection to localhost server.
 */
WebSocket::pointer wsc;

/**
* Initialize a websocket connection to localhost.
*/
void initializeWebsocket() {
	wsc = WebSocket::from_direct("localhost", 9000, "");
}

/**
 * Initialize things for R2Kart.
 * - Winsock (communication to GUI)
 */
void initialize() {
	// Winsock communication
	initializeWinsock();
	//initializeSocketCommunication();
	// Websocket communication
	initializeWebsocket();
}

/**
 * Initialize communication with sensors.
 * 
 * @return		A vector of sensors (available and not available)
 */
std::vector<Sensor *> initializeSensors() {
	std::vector<Sensor *> sensors;

	// Phone IMU
	//Sensor * phoneimu = new PhoneIMUSensor();
	//sensors.push_back(phoneimu);

	// RP Lidar
	Sensor * lidar = new LidarSensor("\\\\.\\com5");
	sensors.push_back(lidar);

	// Kinect
	Sensor * kinect = new KinectSensor();
	sensors.push_back(kinect);

	// Ultrasound
	/*
	std::vector<std::string> locations;
	locations.push_back("Front");
	locations.push_back("Cliff");
	locations.push_back("Left shoulder");
	locations.push_back("Right shoulder");
	locations.push_back("Back");
	Sensor * ultrasound = new UltrasoundSensor("\\\\.\\com11", locations);
	sensors.push_back(ultrasound);
	*/

	return sensors;
}

/**
 * Get sensor data from all sensors and store the data into a bag.
 * 
 * @param slist		Vector of sensors
 * @param sdata		Data bag to fill with sensor data
 */
void getSensorData(std::vector<Sensor *> slist, SensorDataBag * sdata) {
	for (auto sensor = slist.begin(); sensor != slist.end(); sensor++) {
		(*sensor)->getData(sdata);
	}

	return;
}

/**
 * Update the R2 state with data from sensors.
 * 
 * @param sdata		Bag of sensor data
 */
R2State updateState(SensorDataBag * sdata) {
	// Initialize a default state
	R2State state;

	// Highway
	HighwayData * hd = sdata->highway;
	if (hd != NULL && hd->hasData) {
		state.highway = hd->detected;
	}

	// Ultrasound
	UltrasoundData * ud = sdata->ultrasound;
	if (ud != NULL && ud->hasData) {
		for (int i = 0; i < ud->numSensors; i++) {
			state.ultrasound.push_back(ud->distances[i]);
		}
	}

	// TODO Kalman Filter IMU
	// DEBUG position
	IMUData * id = sdata->imu;
	if (id != NULL && id->hasData) {
		state.positionX = 0.0;
		state.positionY = 0.0;
		// Actually orientation Alpha, which is on the Z axis TODO fix this confusion
		state.rotation = id->gyroX * PI / 180.0;
	}

	// Lidar
	LidarData * ld = sdata->lidar;
	if (ld != NULL && ld->hasData) {
		int l = ld->angles.size();
		for (int i = 0; i < l; i++) {
			// DEBUG correction for demo
			double angle = ld->angles[i] - 30.5;
			double distance = ld->distances[i];
			double px = distance * cos(angle * PI / 180.0);
			double py = distance * sin(angle * PI / 180.0);
			state.obstacles.push_back(Obstacle(px, py, 0.5, -1.0));
		}
	}
	/*
	// Kinect
	KinectData * kd = sdata->kinect;
	if (kd != NULL && kd->hasData) {
		if (kd->depth != NULL) {
			// FOV is 58.5 x 46.6
			// Resolution is 640 x 480
			// How many good samples there are (need at least 1/8 * 1/3 * 480 to pass)
			int samplecount[3][640] = {};
			// Minimum depth (to be used as an obstacle)
			double mindepth[3][640];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 640; j++) {
					mindepth[i][j] = 9999.0;
				}
			}

			int index = 0;
			for (int i = 0; i < 480; i++) {
				int section = i / 160;
				for (int j = 0; j < 640; j++) {
					if (kd->depth[index] > 0) {
						double m = kd->depth[index] / 1000.0;
						samplecount[section][j]++;
						if (m < mindepth[section][j]) {
							mindepth[section][j] = m;
						}
					}
					index++;
				}
			}

			// Convert to obstacles
			for (int i = 1; i < 3; i++) {
				for (int j = 0; j < 640; j+=10) {
					if (samplecount[i][j] > 20) {
						double angle = (j - 320.0) * 58.5 / 640.0;
						double distance = mindepth[i][j];
						double px = distance * cos(angle * PI / 180.0);
						double py = distance * sin(angle * PI / 180.0);
						state.obstacles.push_back(Obstacle(px, py, 0.5, i - 1.0));
					}
				}
			}

			// Depth difference
			//state.depthDiff = kd->diff;
		}
	} */

	return state;
}

/**
 * Send the current R2State to the GUI
 * 
 * @param state		The current R2 state
 */
void sendToGui(const R2State &state) {
	// Send data over the connection
	if (wsc != NULL) {
		bool first;

		// Obstacles
		std::string sendObstacles = "[";
		first = true;
		for (int i = 0; i < state.obstacles.size(); i++) {
			Obstacle o = state.obstacles[i];
			if (first) first = false;
			else sendObstacles += ",";

			sendObstacles += "[" + std::to_string(o.positionX) + "," +
				std::to_string(o.positionY) + "," +
				std::to_string(o.height) + "]";
		}
		sendObstacles += "]";

		std::string senddata = std::string("{") +
			//"\"positionX\": " + std::to_string(state.positionX) + "," +
			//"\"positionY\": " + std::to_string(state.positionY) + "," +
			//"\"rotation\": " + std::to_string(state.rotation) + "," +
			"\"obstacles\": " + sendObstacles + "," +
			//"\"highway\": " + std::to_string(state.highway) + "," +
			"\"depthDiff\": " + std::to_string(state.depthDiff) +
		"}";
		wsc->send(senddata);
		wsc->poll();
	}
	return;
}

/**
 * Main function
 */
int main()
{
	// Initialize
	initialize();
	// Get the list of sensors
	std::vector<Sensor *> sensorList = initializeSensors();
	printf("Initialized sensors\n");

	// Forever until exit (escape)
	bool exit = false;
	while (!exit) {
		// Grab the new sensor data
		SensorDataBag * sdata = new SensorDataBag();
		getSensorData(sensorList, sdata);
		// Update the R2 state new sensor data
		R2State state = updateState(sdata);
		// Send the updated state to the GUI
		sendToGui(state);
		// Delete the data bag
		delete sdata;

		// Check if exit
		if (GetAsyncKeyState(VK_ESCAPE)) {
			exit = true;
		}

		// DEBUG wait
		Sleep(50);
	}

	for (auto itr = sensorList.begin(); itr != sensorList.end(); itr++) {
		delete (*itr);
	}

    return 0;
}
