#pragma once

// Source: http://playground.arduino.cc/Interfacing/CPPWindows

#include <exception>
#include <windows.h>

class Serial {
public:
    /// <summary>Initialize Serial communication with the given COM port.</summary>
    Serial(const char* portName, DWORD baud = CBR_115200);

    /// <summary> Close the connection.</summary>
    /// NOTE: for some reason you can't connect again before exiting
    /// the program and running it again.
    ~Serial();

	/// <summary>
	/// Reads exactly `nBytes` bytes into `buffer`. If not enough bytes available,
	/// blocks until they arrive.
	/// </summary>
	void read(char* buffer, unsigned int nBytes);

    /// <summary>
	/// Reads data to `buffer`. If nBytes is greater than the
    /// maximum number of bytes available, it will return only the
    /// bytes available. The function returns -1 when nothing could
    /// be read.
	/// </summary>
    int tryRead(char* buffer, unsigned int nBytes);

    /// <summary>Writes data from a buffer through the Serial connection.</summary>
    void write(const char* buffer, unsigned int nBytes);
		
private:
	// Disable copy constructor
	Serial(const Serial&);

    // Serial comm handler
    HANDLE hSerial;

    // Get various information about the connection
    COMSTAT status;

    // Keep track of last error
    DWORD errors;
};