#include "stdafx.h"
#include "IMUSensor.h"

using easywsclient::WebSocket;

IMUSensor::IMUSensor() : Sensor("IMU"), accX(0.0), accY(0.0), accZ(0.0), rotA(0.0), rotB(0.0), rotC(0.0) {
    wsc = WebSocket::from_direct("localhost", 9040, "");
    lastTime = 0;
}

IMUSensor::~IMUSensor() {
    delete wsc;
}

void IMUSensor::getData(SensorDataBag * sdata) {
	sdata->imu = new IMUData(true, dt, accX, accY, accZ, rotA, rotB, rotC);

	if (wsc != NULL && wsc->getReadyState() != easywsclient::WebSocket::CLOSED) {
		wsc->send("req");
		wsc->poll();
		auto handler = [this](const std::string & message) {
            unsigned long tmpTime = 0;
			double tmpAccX = 0.0;
			double tmpAccY = 0.0;
			double tmpAccZ = 0.0;
			double tmpRotA = 0.0;
			double tmpRotB = 0.0;
			double tmpRotC = 0.0;
			if (sscanf_s(message.c_str(), "%uld %lf %lf %lf %lf %lf %lf",
                &tmpTime, &tmpRotA, &tmpRotB, &tmpRotC, &tmpAccX, &tmpAccY, &tmpAccZ) == 7) {
                // Timestamp
                if (lastTime == 0) {
                    dt = 0.0;
                }
                else {
                    dt = (tmpTime - lastTime) / 1000.0;
                }
                lastTime = tmpTime;
				// Orientation data
				rotA = tmpRotA;
				rotB = tmpRotB;
				rotC = tmpRotC;
				// Acceleration data
				accX = tmpAccX;
				accY = tmpAccY;
				accZ = tmpAccZ;
			}
		};
		wsc->dispatch(handler);
	}
}
