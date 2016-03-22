#pragma once
#include "stdafx.h"
#include <tuple>
#include <unordered_map>

class Barcode {
public:
	//tuple<barcode x, barcode y, barcode orientation angle, x of point , y of point in front of barcode>
	typedef std::tuple<double, double, double, double, double> barcodeData;
	Barcode();
	Barcode::barcodeData Barcode::getBarcodeData(int id);
	void Barcode::addBarcodeData(int id, Barcode::barcodeData data);
private:
	std::unordered_map < int, Barcode::barcodeData> barcodeMap;
};