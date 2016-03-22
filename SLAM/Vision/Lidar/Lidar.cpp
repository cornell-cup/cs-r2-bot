#include "stdafx.h"
#include "Lidar.h"

struct Packet {
	unsigned char indexByte;
	unsigned char speedBytes[2];
	unsigned char dataBytes[4][4];
};

int buildWord(unsigned char byte0, unsigned char byte1) {
	// Build a word from two bytes assuming little-endian
	// formatting --> [byte1 byte0]
	return (byte1 << 8) | byte0;
}

int calculateChecksum(unsigned char* data) {
	int checksum = 0;
	for (int i = 0; i < 10; i++) {
		checksum = (checksum << 1) + buildWord(data[2*i], data[2*i+1]);
	}
	checksum = ((checksum >> 15) + checksum) & 0x7FFF;
	return checksum;
}

// TODO: timeout
Packet readPacket(Serial& device) {
	Packet p;
	while (true) {
		// Packet starts with 0xFA
		unsigned char initialByte;
		device.read((char*) &initialByte, 1);
		if (initialByte != 0xFA) { continue; }

		// Second byte is between 0xA0 and 0xF9
		device.read((char*) &p.indexByte, 1);
		if (!(p.indexByte <= 0xF9 && p.indexByte >= 0xA0)) { continue; }

		// Collect raw packet data and do checksum
		unsigned char rawData[20];
		rawData[0] = initialByte;
		rawData[1] = p.indexByte;
		device.read((char*) rawData+2, 18);
		unsigned char checksumBytes[2];
		device.read((char*) checksumBytes, 2);
		if (calculateChecksum(rawData) != buildWord(checksumBytes[0], checksumBytes[1])) { continue; }

		// Return packet data
		std::memcpy(&p.speedBytes, rawData + 2, sizeof(p.speedBytes));
		std::memcpy(&p.dataBytes[0], rawData + 4, sizeof(p.dataBytes[0]));
		std::memcpy(&p.dataBytes[1], rawData + 8, sizeof(p.dataBytes[1]));
		std::memcpy(&p.dataBytes[2], rawData + 12, sizeof(p.dataBytes[2]));
		std::memcpy(&p.dataBytes[3], rawData + 16, sizeof(p.dataBytes[3]));
		return p;
	}
}

std::vector<PolarPoint> parsePacket(const Packet& packet, int intensityFloor=0,
		float intensityCeiling=std::numeric_limits<float>::infinity()) {
	std::vector<PolarPoint> pointData;

	// The 4 points in the packet are at consecutive degrees, and start with `baseDegree`
	int baseDegree = (packet.indexByte - 0xA0) * 4;
	//float rpm = buildWord(packet.speedBytes[0], packet.speedBytes[1]) / 64.0f;
	for (int i = 0; i < 4; i++) {
		const unsigned char* bytes = packet.dataBytes[i];
		int intensity = buildWord(bytes[2], bytes[3]);
		char badDataFlag = bytes[1] & 0x80;
		char mediocreDataFlag = bytes[1] & 0x40;
		int degree = baseDegree + i;
		if (!mediocreDataFlag && !badDataFlag && (intensityFloor <= intensity) && (intensity < intensityCeiling)) {
			int depth = buildWord(bytes[0], bytes[1]);
			PolarPoint point = {degree, depth};
			pointData.push_back(point);
		}
	}
	// Implementation note: When data is bad, no point
	// is returned. This can cause refresh issues if
    // one's implementation of the visualizer does not
    // reset stale points. The original version took
    // advantage of bad points being returned as (0, 0)
    // to effect the refresh.
	return pointData;
}


Lidar::Lidar(const char* comPort) : device(comPort) { }

std::vector<PolarPoint> Lidar::readData() {
	return parsePacket(readPacket(device), 10, 5000);
}