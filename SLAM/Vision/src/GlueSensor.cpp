#include "stdafx.h"
#include "GlueSensor.h"

using easywsclient::WebSocket;

GlueSensor::GlueSensor(std::string inPort) : Sensor("Encoder") {
	wsc = WebSocket::from_direct("localhost", 9010, "");
}

GlueSensor::~GlueSensor() {
	// TODO clean up
}

void GlueSensor::getData(SensorDataBag * sdata) {

	if (wsc != NULL && wsc->getReadyState() != easywsclient::WebSocket::CLOSED) {
		wsc->send("req");
		wsc->poll();
		auto handler = [this](const std::string & message) {
			// Route data accordingly here
			double tmpAccX = 0.0;
			double tmpAccY = 0.0;
			double tmpAccZ = 0.0;
			double tmpRotA = 0.0;
			double tmpRotB = 0.0;
			double tmpRotC = 0.0;
			if (sscanf_s(message.c_str(), "0 %lf %lf %lf", &tmpAccX, &tmpAccY, &tmpAccZ) == 3) {
				// Acceleration data
			}
			else if (sscanf_s(message.c_str(), "1 %lf %lf %lf", &tmpRotA, &tmpRotB, &tmpRotC) == 3) {
				// Orientation data
			}
		};
		wsc->dispatch(handler);
	}
}
