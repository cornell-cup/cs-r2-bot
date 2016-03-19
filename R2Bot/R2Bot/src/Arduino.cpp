#include "stdafx.h"
#include "Arduino.h"
#include <cmath>

namespace R2D2 {

	// Utility method used by PowerMonitor::update()
	float parseInput(unsigned char* input) {
		// Each input is two bytes; the first is the integral part, the second is the decimal part
		return input[0] + (input[1] / 100.0f);
	}
	
	Arduino::Arduino(const std::string& comPort) : serial(comPort.c_str(), CBR_57600) { }
	
	PowerData Arduino::getPowerData() {
		// Data is requested using the '%' character.
		// A series of 8 consecutive bytes, two for each value, is returned in this order:
		// * Battery %
		// * Voltage
		// * Regulated Current (electronics)
		// * Unregulated Current (motor)
		// These values are multiplied by 9 to preserve precision, so we divide by 9 and round to the
		// nearest tenth to get their values.
		// Note that we read 20 bytes, the first 8 are power, and the next 12 are IMU values (ignored).
		PowerData data;
		serial.write("%", 1);
		unsigned char buf[20];
		serial.read((char*) buf, 20);
		data.batteryPercent     = parseInput(buf + 0);
		data.voltage            = parseInput(buf + 2);
		data.regulatedCurrent   = parseInput(buf + 4);
		data.unregulatedCurrent = parseInput(buf + 6);
		return data;
	}

	ImuData Arduino::getImuData() {
		ImuData data;
		serial.write("%", 1);
		unsigned char buf[20];
		int i = 0;
		do {
			Log::debug("Read %d bytes so far", i);
			i += serial.tryRead((char*)buf, 20 - i);
			Sleep(50);
			serial.write("%", 1);
		} while (i < 20);
		//serial.read((char*) buf, 20);
		data.yaw   = parseInput(buf + 8);
		data.pitch = parseInput(buf + 10);
		data.roll  = parseInput(buf + 12);
		data.ax    = parseInput(buf + 14);
		data.ay    = parseInput(buf + 16);
		data.az    = parseInput(buf + 18);
		Log::debug("y %f p %f r %f x %f y %f z %f", data.yaw, data.pitch, data.roll, data.ax, data.ay, data.az);
		return data;
	}

	
	PowerMonitor::PowerMonitor(Arduino& arduino, BaseConnection& base)
		: arduino(arduino), baseConn(base) { }

	void PowerMonitor::operator()() {
		PowerData data = arduino.getPowerData();
		baseConn.updatePowerStats(
			data.batteryPercent, data.voltage, data.regulatedCurrent, data.unregulatedCurrent);
	}

	ImuMonitor::ImuMonitor(Arduino& arduino) : arduino(arduino) { }

	void ImuMonitor::operator()() {
		ImuData data = arduino.getImuData();
	}
}