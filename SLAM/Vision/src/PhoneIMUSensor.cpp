#include "stdafx.h"
#include "PhoneIMUSensor.h"

using easywsclient::WebSocket;

PhoneIMUSensor::PhoneIMUSensor() : Sensor("Phone IMU"), accX(0.0), accY(0.0), accZ(0.0), rotA(0.0), rotB(0.0), rotC(0.0) {
	wsc = WebSocket::from_direct("localhost", 9040, "");
}

PhoneIMUSensor::~PhoneIMUSensor() {
	delete wsc;
}

void PhoneIMUSensor::getData(SensorDataBag * sdata) {
	sdata->imu = new IMUData(true, accX, accY, accZ, rotA, rotB, rotC);

	if (wsc != NULL && wsc->getReadyState() != easywsclient::WebSocket::CLOSED) {
		wsc->send("req");
		wsc->poll();
		auto handler = [this](const std::string & message) {
			double tmpAccX = 0.0;
			double tmpAccY = 0.0;
			double tmpAccZ = 0.0;
			double tmpRotA = 0.0;
			double tmpRotB = 0.0;
			double tmpRotC = 0.0;
			if (sscanf_s(message.c_str(), "0 %lf %lf %lf", &tmpAccX, &tmpAccY, &tmpAccZ) == 3) {
				// Acceleration data
				accX = tmpAccX;
				accY = tmpAccY;
				accZ = tmpAccZ;
			}
			else if (sscanf_s(message.c_str(), "1 %lf %lf %lf", &tmpRotA, &tmpRotB, &tmpRotC) == 3) {
				// Orientation data
				rotA = tmpRotA;
				rotB = tmpRotB;
				rotC = tmpRotC;
			}
		};
		wsc->dispatch(handler);
	}
}

void PhoneIMUSensor::handle_message(const std::string & message) {
	double tmpAccX = 0.0;
	double tmpAccY = 0.0;
	double tmpAccZ = 0.0;
	double tmpRotA = 0.0;
	double tmpRotB = 0.0;
	double tmpRotC = 0.0;
	if (sscanf_s(message.c_str(), "0 %lf %lf %lf", tmpAccX, tmpAccY, tmpAccZ) == 3) {
		// Acceleration data
		accX = tmpAccX;
		accY = tmpAccY;
		accZ = tmpAccZ;
	}
	else if (sscanf_s(message.c_str(), "1 %lf %lf %lf", tmpRotA, tmpRotB, tmpRotC) == 3) {
		// Orientation data
		rotA = tmpRotA;
		rotB = tmpRotB;
		rotC = tmpRotC;
	}
}
