// Source: http://playground.arduino.cc/Interfacing/CPPWindows
// For more details, see http://gtk-study-korea.googlecode.com/files/serial-win.pdf

#include "stdafx.h"
#include "Serial.h"
#include <exception>
#include <stdexcept>
#include <algorithm>

const char FAKE_SERIAL[] = "FAKE_SERIAL";

Serial::Serial(const char* portName, DWORD baud) {
	// Test if this serial is real
	if (std::strcmp(portName, FAKE_SERIAL) == 0) {
		this->hSerial = INVALID_HANDLE_VALUE;
		return;
	}

    // Try to connect to the given port throuh CreateFile.
    this->hSerial = CreateFile(
			portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    // Check if the connection was successful.
    if (this->hSerial == INVALID_HANDLE_VALUE) {
        if(GetLastError() == ERROR_FILE_NOT_FOUND) {
			throw std::runtime_error("Unable to find serial port");
        } else {
			throw std::runtime_error("Failed to connect to serial port");
        }
    }

    // Try to set the comm parameters.
    DCB dcbSerialParams = {0};
    if (!GetCommState(this->hSerial, &dcbSerialParams))
        throw std::runtime_error("Failed to get current serial parameters.");

    // Define serial connection parameters for the arduino board.
	dcbSerialParams.BaudRate = baud;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
		throw std::runtime_error("Could not set Serial Port parameters");
}

Serial::~Serial() {
	CloseHandle(this->hSerial);
}

void Serial::read(char* buffer, unsigned int nBytes) {
	while (this->hSerial == INVALID_HANDLE_VALUE) { }

	// Read exactly nBytes from serial.
	DWORD bytesRead;
    if (!ReadFile(this->hSerial, buffer, nBytes, &bytesRead, NULL) && bytesRead != nBytes)
		throw std::runtime_error("Failed to read from serial port!");
}

int Serial::tryRead(char* buffer, unsigned int nBytes) {
	if (this->hSerial == INVALID_HANDLE_VALUE) { return 0; }

    // Use the ClearCommError function to get status info on the Serial port.
    ClearCommError(this->hSerial, &this->errors, &this->status);

    // Check if there is something to read.
    if (this->status.cbInQue > 0) {
        // Take only as many bytes as available, or the read will block.
		unsigned int toRead = std::min<unsigned int>(this->status.cbInQue, nBytes);
		DWORD bytesRead;
        if (!ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
			throw std::runtime_error("Failed to read from serial port");
        return bytesRead;
    }

    // If nothing has been read, or that an error was detected, then return 0
    return 0;
}

void Serial::write(const char* buffer, unsigned int nBytes) {
	if (this->hSerial == INVALID_HANDLE_VALUE) { return; }

    // Try to write the buffer on the Serial port.
    DWORD bytesSend;
    if (!WriteFile(this->hSerial, (void*) buffer, nBytes, &bytesSend, 0)) {
        ClearCommError(this->hSerial, &this->errors, &this->status);
		throw std::runtime_error("Failed to write to serial port");
    }
}