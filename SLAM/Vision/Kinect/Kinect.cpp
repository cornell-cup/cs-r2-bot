#include "stdafx.h"
#include "Kinect.h"
#include <thread>

#include <Windows.h>
#include <Ole2.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <opencv2/opencv.hpp>

#include <zbar.h>
#include <zbar\Image.h>
#include <zbar\ImageScanner.h>

#include "TwoCirclesIntersections.h"
#include "Barcode.h"

#include <thread>

//#include <boost/bind.hpp>
//#include <boost/signal.hpp>


R2D2::BotPosition botPos;
//stores barcode map coordinates
Barcode barcodeMap;
float headRotationAngle = 0; // getHeadRotationAngle() ; //David implement this

const float FEET_PER_METER = 3.28084;

const float KINECT_HEIGHT = 0.9; // meters
const float MIN_HEIGHT = -(KINECT_HEIGHT - 0.01); // 1 cm above the ground
const float MAX_HEIGHT = 0.10; // 10 cm above the kinect
//const int CV_CAP_OPENNI = 900;



#define RGB_WIDTH		1280	// Width of RGB image
#define RGB_HEIGHT		960		// Height of RGB image
#define DEPTH_WIDTH		640		// Width of Depth image
#define DEPTH_HEIGHT	480		// Height of Depth image

#define CHANNEL			3		// Number of color channels in image
//#define OFF 430.0				// Offset in millimeters between 2 signposts
#define PI 3.14159265			// Constant value for Pi because maths
#define mmToFt	0.00328084		// Conversion factor from mm to ft

BYTE buf[DEPTH_WIDTH*DEPTH_HEIGHT*CHANNEL];		// Byte array to assist in converting depth data into a gray-scale image
USHORT depthBuf[DEPTH_WIDTH*DEPTH_HEIGHT];		// Global depth data buffer

using namespace std;
using namespace zbar;
using namespace cv;
//using namespace boost;

// Kinect variables
HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera
HANDLE depthStream;            // The identifier of the Kinect's depth Camera
HANDLE nextRGBFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event indicating that a new color frame is ready
HANDLE nextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event indicating that a new depth frame is ready
INuiSensor* sensor;            // Pointer to the kinect sensor

// Handles for threads that concurrently grab RGB and Depth frames from the Kinect
HANDLE rgbThread;
HANDLE depthThread;

// Variable used to evaluate if operation is successful or not
HRESULT hr;

// Zbar class used to scan for barcodes
ImageScanner scanner;

// Simple boolean to control program execution
bool run = true;
bool notSameBarcodes = true;


/*
* Searches for a functioning kinect sensor, initializes it, then opens RGB
* and depth streams
*/

Kinect::Kinect(R2D2::BotPosition& pos) : botPos(pos) {
	//barcodeMap.addBarcodeData(215639, std::make_tuple((250.0*mmToFt), 0, 0, (300.0*mmToFt), (10.0*mmToFt))); // in ft
	//barcodeMap.addBarcodeData(348531, std::make_tuple((765.0*mmToFt), 0, 0, (300.0*mmToFt), (10.0*mmToFt)));
	barcodeMap.addBarcodeData(215639, std::make_tuple(30, 29, 0, 30, 30)); // in ft
	barcodeMap.addBarcodeData(348531, std::make_tuple(31, 29, 0, 30, 30));
	//barcodeMap.addBarcodeData(215639, std::make_tuple(250, 0, 0, 300, 10)); // in mm
	//barcodeMap.addBarcodeData(348531, std::make_tuple(765, 0, 0, 300, 10));
	if (!initKinect()) { std::cerr << "Failed init"; }
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_X_DENSITY, 2);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_Y_DENSITY, 0);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
	scanner.set_config(ZBAR_I25, ZBAR_CFG_ENABLE, 1);
	dataUsed = true;
	//spawn child threads
	startThreads();


}



bool Kinect::initKinect() {
	// Get a working kinect sensor
	std::cout << "  Get a working kinect sensor...";
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) {
		std::cerr << std::endl << "ERROR: Unable to find kinect sensors!" << std::endl;
		Sleep(3000);
		return false;
	}
	if (NuiCreateSensorByIndex(0, &sensor) < 0) {
		std::cerr << std::endl << "ERROR: Unable to create a kinect sensor instance!" << std::endl;
		Sleep(3000);
		return false;
	}
	std::cout << "done" << std::endl;

	// Initialize sensor
	std::cout << "  Initialize kinect sensor...";
	hr = sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	if (FAILED(hr)) {
		std::cerr << std::endl << "ERROR: Unable to initialize sensor! " << hr << std::endl;
		Sleep(3000);
		return false;
	}
	std::cout << "done" << std::endl;

	// Open RGB frame stream
	std::cout << "  Open color frame stream...";
	hr = sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,           // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_1280x960,  // Image resolution
		0,								// Image stream flags, e.g. near mode
		2,								// Number of frames to buffer
		nextRGBFrameEvent,				// Event handle
		&rgbStream);
	if (FAILED(hr)) {
		std::cerr << std::endl << "ERROR: Unable to open color frame stream!" << std::endl;
		Sleep(3000);
		return false;
	}
	std::cout << "done" << std::endl;

	// Open depth frame stream
	std::cout << "  Open depth frame stream...";
	hr = sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,           // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,   // Image resolution
		0,								// Image stream flags, e.g. near mode
		2,								// Number of frames to buffer
		nextDepthFrameEvent,			// Event handle
		&depthStream);
	if (FAILED(hr)) {
		std::cerr << std::endl << "ERROR: Unable to open depth frame stream!" << std::endl;
		Sleep(3000);
		return false;
	}
	std::cout << "done" << std::endl;

	return sensor != NULL;
}



/*
* Simple helper function that grabs depth information for an RGB image
* coordinate from a global depth buffer
*
* @param x x-coordinate of point in RGB image to grab depth data for
* @param y y-coordinate of point in RGB image to grab depth data for
*/
USHORT Kinect::getDepth(int x, int y){
	USHORT depth;
	// Get depth data of one pixel in milimeters
	if (RGB_WIDTH == DEPTH_WIDTH) {
		depth = NuiDepthPixelToDepth(*(depthBuf + y*DEPTH_WIDTH + x));
	}
	else {
		depth = NuiDepthPixelToDepth(*(depthBuf + (y / 2)*DEPTH_WIDTH + (x / 2)));
	}
	return depth;
}

/*
* Simple helper function that grabs depth information for an RGB image
* coordinate from a global depth buffer
*
* @param img Gray-scale image within which to scan for barcodes
*/
void Kinect::scanForBarcode(Mat img)
{
	// Check for errors
	if (img.empty()) {
		std::cout << "Error occured, image not read correctly" << std::endl;
	}


	// Convert image into a format usable by Zbar
	int imgWidth = img.cols;
	int imgHeight = img.rows;
	Image image(imgWidth, imgHeight, "Y800", (uchar*)img.data, imgWidth*imgHeight);

	// Scan the image for barcodes
	scanner.scan(image);

	// Initialize variables for position calculation
	vector<double> vdepth;
	vector<double> voff;
	vector<double> vxoff; //xOff in pixels
	vector<int> id;
	vector<std::tuple<int, int>> centers; //circle centers
	vector<Barcode::barcodeData> barcodeData;
	double c, c1, a;
	double ints_x, ints_y;
	boolean botPositionFound = false;

	// Iterate through each of the detected barcodes and perform useful calculations
	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
		/*
		//get barcode ID
		int id = stoi(symbol->get_data());
		Barcode::barcodeData data = barcodeMap.getBarcodeData(id);
		bool barcodeDataFound = get<0>(data) != -1;
		if (get<2>(data) == -999) {
			continue;
		}
		else{
			barcodeData.push_back(data);
		}
		*/
		// Display decoded barcode data
		std::cout << "Code: " << symbol->get_data();
		id.push_back(std::stoi(symbol->get_data()));

		// Initialize variables for calculating center of detected barcode
		int x_max = 0;
		int x_min = RGB_WIDTH;
		int y_max = 0;
		int y_min = RGB_HEIGHT;
		int x_avg, y_avg;

		// Initialize a vector to hold all the points associated with the current barcode
		vector<Point> vp;

		// Iterate through all the points associated with the current barcode
		int n = symbol->get_location_size();
		for (int i = 0; i < n; i++){
			int x = symbol->get_location_x(i);
			int y = symbol->get_location_y(i);

			// Store point in vector for use in future calculations
			vp.push_back(Point(x, y));

			// Get the max and min xy coordinates
			x_max = max(x_max, x);
			x_min = min(x_min, x);
			y_max = max(y_max, y);
			y_min = min(y_min, y);

		}

		// Get central point of detected barcode as the average of the max and min xy 
		// coordinates of all the points assocaited with the barcode
		x_avg = (x_max + x_min) / 2;
		y_avg = (y_max + y_min) / 2;
		centers.push_back(std::make_tuple(x_avg, y_avg));

		// Get the depth of the 
		USHORT depth = getDepth(x_avg, y_avg);
		
		// Get x offset
		double xOff = abs(x_avg - ((RGB_WIDTH - 1) / 2));
		double x_dis = (xOff / (RGB_WIDTH / 2))*0.543*depth; // 0.543 = tan (57/2); 57 degree is the viewing angle of Kinect
		vxoff.push_back(xOff);

		//  print data
		std::cout << " Center: (" << x_avg << ", " << y_avg << ") " << "Depth: " << depth << "mm xOff: " << x_dis << "mm" << std::endl;

		//  save depth and xoff for calculation
		vdepth.push_back(depth);
		voff.push_back(x_dis);

	}

	double r1, r2, OFF, c2;
	double ints_x1, ints_y1, ints_x2, ints_y2;

	//if 2 barcodes are found
	if (id.size() >= 2) {
		double avg_depth = (vdepth[0] + vdepth[1]) / 2;
		voff[0] = (vxoff[0] / (RGB_WIDTH / 2))*0.543*avg_depth;
		voff[1] = (vxoff[1] / (RGB_WIDTH / 2))*0.543*avg_depth;

		for (int i = 0; i<2; i++){
		}
		//Barcode barcodeMap; //global
		std::cout << "id[0] " << id[0] << std::endl;
		std::cout << "id[1] " << id[1] << std::endl;
		Barcode::barcodeData data = barcodeMap.getBarcodeData(id[0]);
		Barcode::barcodeData data2 = barcodeMap.getBarcodeData(id[1]);
		std::cout << "barcode1 x: " << std::get<0>(data) << ",  barcode1 y: " << std::get<1>(data) << std::endl;
		std::cout << "barcode2 x: " << std::get<0>(data2) << ",  barcode2 y: " << std::get<1>(data2) << std::endl;

		//calculate distance between the two barcode coordinates
		// physical hardcoded distance between barcode centers in ft
		OFF = sqrt(pow((std::get<0>(data) -std::get<0>(data2)), 2.0)
			+ pow((std::get<1>(data) -std::get<1>(data2)), 2.0));

		// distance using pixel distance between barcode centers, converting from pixel to mm; 1 pixel = 0.264583 mm ??
		//OFF= 0.264583*( sqrt(pow((std::get<0>(centers[0]) - std::get<0>(centers[1])), 2.0) 
		//	+ pow((std::get<1>(centers[0]) - std::get<1>(centers[1])), 2.0)) );

		double x_kinect;
		double y_kinect;
		bool dataFound = false;
		if (vdepth.size() == 2 && (int)vdepth[0] != 0 && (int)vdepth[1] != 0){
			// kinect offset calculation
			c1 = sqrt(abs((pow(vdepth[0], 2.0) + pow(voff[0], 2.0) - pow(vdepth[1], 2.0) - pow(voff[1], 2.0) - pow(OFF, 2.0)) / (-2 * OFF)));
			c = sqrt(pow(vdepth[0], 2.0) + pow(voff[0], 2.0) - pow(c1, 2.0));
			//get botPos.theta
			a = 0; //angle between camera plane and barcode plane
			double oh = (abs(vdepth[1] - vdepth[0])*mmToFt / abs(OFF));
			if (oh <= 1 && oh >= -1) { //asin(x): x must be in [-1,1]
				a = asin(oh) * 180 / PI;
				//std::cout << "i " << i << std::endl;			
			}
			std::cout << "(vdepth[1] - vdepth[0]): " << abs(vdepth[1] - vdepth[0])*mmToFt << "ft,  OFF: " << OFF << "ft,  Orientation: " << a << "degrees" << std::endl;
			botPos.theta = std::get<2>(data)+ 180 - a - headRotationAngle; //angleBarcode facing +180 - angle between camera plane and barcode plane- headRotationAngle
			while (botPos.theta > 360){
				botPos.theta = botPos.theta - 360;
			}
			while (botPos.theta <0){
				botPos.theta = botPos.theta + 360;
			}
			std::cout << "c: " << c << "mm,  c1: " << c1 << "mm,  Orientation: " << a << "degrees" << std::endl;
			// point of kinect
			x_kinect = c1 + 250; //HARDCODED 250 NEEDS TO CHANGE
			y_kinect = c;
			dataFound = true;

			std::cout << "vdepth[0]: " << vdepth[0] << "mm,  voff[0]: " << voff[0] << "mm" << std::endl;
			std::cout << "vdepth[1]: " << vdepth[1] << "mm,  voff[1]: " << voff[1] << "mm" << std::endl;

			r1 = sqrt(pow(vdepth[0], 2.0) + pow(voff[0], 2.0));
			r2 = sqrt(pow(vdepth[1], 2.0) + pow(voff[1], 2.0));
			c2 = OFF - c1;
			//r1 = sqrt(pow(c1, 2.0) + pow(c, 2.0));
			//r2 = sqrt(pow(c2, 2.0) + pow(c, 2.0));
			//convert from mm (kinect unit) to foot (map unit)
			r1 = r1*mmToFt;
			r2 = r2*mmToFt;
			//std::cout << "c1: " << c1 << "mm,  c2: " << c2 << "mm, " << "OFF: " << OFF << std::endl;
			//std::cout << "r1: " << r1 << "mm,  r2: " << r2 << "mm" <<std::endl;
			std::cout << "r1: " << r1 << "ft,  r2: " << r2 << "ft" << std::endl;

			//find 2 circle intersections
			int result = TwoCirclesIntersections::two_circles_intersections(
				std::get<0>(data), std::get<1>(data), r1,
				std::get<0>(data2), std::get<1>(data2), r2,
				&ints_x1, &ints_y1,
				&ints_x2, &ints_y2);

			std::cout << "2 intersections found(=1)? " << result << std::endl;
			if (result == 1) {
				//xf , yf is a point in front of the barcodes
				double xf = std::get<3>(data); double yf = std::get<4>(data);
				double dist1 = sqrt(pow((ints_x1 - xf), 2.0) + pow((ints_y1 - yf), 2.0));
				double dist2 = sqrt(pow((ints_x2 - xf), 2.0) + pow((ints_y2 - yf), 2.0));
				if (dist1 < dist2){
					ints_x = ints_x1;
					ints_y = ints_y1;
					botPositionFound = true;
				}
				else{
					ints_x = ints_x2;
					ints_y = ints_y2;
					botPositionFound = true;
				}
				std::cout << "intersection for kinect location: " << ints_x << ", " << ints_y << std::endl;
				std::cout << std::endl;
			}
		}
	}


	waitKey(1);
	//clean up
	image.set_data(NULL, 0);
	//if (dataFound && notSameBarcodes)
	if (botPositionFound)
	{
		//update botPost
		botPos.x = ints_x; //need to change these updated values to actual variables
		botPos.y = ints_y;
		//botPos.theta //already obtained
		//barcodeFoundHandler(5, 20); // You'll need different parameters - basically all info you want to pass to visiondata
		//notSameBarcodes = false; // Make this false when checking if barcodes are same - not here 
		botPositionFound = false;
	}
	return;
}

/*
* Grabs RGB frames from the kinect, displays them as an RGB image. Also calls
* scanForBarcode() to look for barcodes in the current image
*/
void Kinect::updateColorImage() {
	NUI_IMAGE_FRAME imageFrame;	// Structure containing frame metadata
	NUI_LOCKED_RECT LockedRect;	// Contains a pointer to the actual data

	// Grab RGB frame data from kinect, return immediately if something goes wrong
	hr = sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame);
	if (FAILED(hr)) {
		std::cerr << "ERROR : Get new color frame failed!" << std::endl;
		return;
	}

	// If RGB frame grabbed successfully, perform additional processing steps
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) {
		BYTE* curr = (BYTE*)LockedRect.pBits;
		Mat img(RGB_HEIGHT, RGB_WIDTH, CV_8UC4, curr);

		// Convert RGB image into grayscale because Zbar only accepts gray-scale images
		Mat imgGray;
		cvtColor(img, imgGray, CV_BGR2GRAY);

		// Crop image to reduce processing time
		Mat imgCrop = imgGray(Rect(0, RGB_HEIGHT / 2, RGB_WIDTH, 400));

		// Search for barcodes in cropped gray-scale image, output FPS information
		clock_t init = clock();
		scanForBarcode(imgCrop);
		clock_t final = clock() - init;
		std::cout << "Current FPS: " << (double)CLOCKS_PER_SEC / (double)final << std::endl;

		waitKey(1);
	}
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

/*
* Grabs depth frames from the kinect, stores depth data in a global buffer to
* be read by other functions. Also converts depth data into a viewable gray-scale
* image
*/
void Kinect::updateDepthImage() {
	NUI_IMAGE_FRAME imageFrame;	// Structure containing frame metadata
	NUI_LOCKED_RECT LockedRect;	// Contains a pointer to the actual data

	// Grab depth frame data from kinect, return immediately if something goes wrong
	hr = sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame);
	if (FAILED(hr)) {
		std::cerr << "ERROR : Get new depth frame failed!" << std::endl;
		return;
	}

	// If depth frame grabbed successfully, perform additional processing steps
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) {
		USHORT* curr = (USHORT*)LockedRect.pBits;
		for (int i = 0; i < DEPTH_WIDTH*DEPTH_HEIGHT; i++)
		{
			// Store depth data in a global buffer
			depthBuf[i] = curr[i];
		}
		waitKey(1);
	}
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}

/*
* Helper thread that runs concurrently in the background, calls updateColorImage()
* every time a new nextRGBFrameEvent is received (kinect has a new RGB frame ready)
*/
DWORD WINAPI Kinect::rgbFunc(LPVOID pParam) {
	UNREFERENCED_PARAMETER(pParam);
	while (run) {
		if (WaitForSingleObject(nextRGBFrameEvent, INFINITE) == WAIT_OBJECT_0) {
			updateColorImage();
		}
	}
	return 0;
}

/*
* Helper thread that runs concurrently in the background, calls updateDepthImage()
* every time a new nextDepthFrameEvent is received (kinect has a new depth frame ready)
*/
DWORD WINAPI Kinect::depthFunc(LPVOID pParam) {
	UNREFERENCED_PARAMETER(pParam);
	while (run) {
		if (WaitForSingleObject(nextDepthFrameEvent, INFINITE) == WAIT_OBJECT_0) {
			updateDepthImage();
		}
	}
	return 0;
}

void Kinect::startThreads() {
	//rgbThread = CreateThread(NULL, 0, rgbFunc, rgbStream, 0, NULL);
	//depthThread = CreateThread(NULL, 0, depthFunc, depthStream, 0, NULL);
	std::thread([this] {
		rgbFunc(rgbStream);
	}).detach();

	std::thread([this] {
		depthFunc(depthStream);
	}).detach();
}

/*
* Waits on keyboard input, properly shuts down when the 'Esc' key is pressed
* This doesn't work because threads aren't killed. Manually terminate code for now
*/
void Kinect::processKey() {
	char key = (waitKey(1) & 255);
	if (key == 27) {
		// Exit on escape key
		std::cout << "SHUT DOWN" << std::endl;
		run = false;

		// Clean up
		//CloseHandle(rgbThread);
		//CloseHandle(depthThread);
		destroyAllWindows();
		Sleep(100);
		NuiShutdown();
		exit(0);
	}
}




int Kinect::initBarcode()
{
	if (!initKinect()) return 1;

	scanner.set_config(ZBAR_NONE, ZBAR_CFG_X_DENSITY, 2);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_Y_DENSITY, 0);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
	scanner.set_config(ZBAR_I25, ZBAR_CFG_ENABLE, 1);

	//rgbThread = CreateThread(NULL, 0, rgbFunc, rgbStream, 0, NULL);
	//depthThread = CreateThread(NULL, 0, depthFunc, depthStream, 0, NULL);

	std::thread([this] {
		rgbFunc(rgbStream);
	}).detach();

	std::thread([this] {
		depthFunc(depthStream);
	}).detach();

	return 0;

}
int Kinect::testingInit(int argc, char* argv[]) {

	// Kinect initialization
	std::cout << "Init Kinect..." << std::endl;
	if (!initKinect()) return 1;

	// OpenCV initialization
	std::cout << "Init OpenCV...";
	namedWindow("RGB", CV_WINDOW_AUTOSIZE);
	namedWindow("Depth", CV_WINDOW_AUTOSIZE);
	namedWindow("Detection", CV_WINDOW_AUTOSIZE);
	namedWindow("Map", CV_WINDOW_AUTOSIZE);
	std::cout << "done" << std::endl;

	// Zbar initialization
	std::cout << "Init Zbar...";
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_X_DENSITY, 2);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_Y_DENSITY, 0);
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
	scanner.set_config(ZBAR_I25, ZBAR_CFG_ENABLE, 1);
	std::cout << "done" << std::endl;

	// Spawn threads to update rgb and depth images
	std::cout << "Spawn child threads...";
	//rgbThread = CreateThread(NULL, 0, rgbFunc, rgbStream, 0, NULL);
	//depthThread = CreateThread(NULL, 0, depthFunc, depthStream, 0, NULL);

	std::thread([this] {
		rgbFunc(rgbStream);
	}).detach();

	std::thread([this] {
		depthFunc(depthStream);
	}).detach();
	std::cout << "done" << std::endl;

	// Spin while polling for keyboard input
	while (run) {
		processKey();
	}
	return 0;
}