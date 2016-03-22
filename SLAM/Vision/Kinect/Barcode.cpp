#include "stdafx.h"
#include "Barcode.h"
#include <iostream>


using namespace std;

Barcode::Barcode(void){
}

void Barcode::addBarcodeData(int id, Barcode::barcodeData data) {
	barcodeMap[id] = data;
}


Barcode::barcodeData Barcode::getBarcodeData(int id) {
	try {
		Barcode::barcodeData data = barcodeMap.at(id);
		return data;

	}
	catch (std::exception e) {
		Barcode::barcodeData data = std::make_tuple(0, 0, -999, 0, 0);
		return data;
		std::cout << "exception thrown" << std::endl;

	}
}
//
//int main() {
//	Barcode barcode;
//	barcode.addBarcodeData(215639, std::make_tuple(38, 32, 0));
//	barcode.addBarcodeData(567234, std::make_tuple(39, 31, 0));
//	Barcode::barcodeData data = barcode.getBarcodeData(215639);
//	Barcode::barcodeData data2 = barcode.getBarcodeData(567234);
//	std::cout << "first data" << std::get<0>(data) << std::endl;
//	std::cout << "second data" << std::get<0>(data2) << std::endl;
//}