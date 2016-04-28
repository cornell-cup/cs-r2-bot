#include "stdafx.h"
#include "ConstantKalmanFilter.h"

ConstantKalmanFilter::ConstantKalmanFilter(double inInitialState, double inInitialCovariance, double inProcessCovariance, double inMeasurementCovariance) : vState(inInitialState), mCovariance(inInitialCovariance), mProcess(inProcessCovariance), mMeasurement(inMeasurementCovariance) {
}

ConstantKalmanFilter::~ConstantKalmanFilter() {
    
}

double ConstantKalmanFilter::update(double inMeasurement) {
	// Short names
	double xp = vState;
	double pp = mCovariance;

	double A = mTransition;
	double AT = A;
	double B = mControl;
	double H = mObservation;
	double HT = H;
	double I = 1.0;
	double Q = mProcess;
	double R = mMeasurement;

	// Predict the next state
	double vPreState = (A * xp) + (B * 0.0);

	// Predict covariance
	double mPreCovariance = (A * pp * AT) + Q;

	// Compare prediction with measurement
	double vCmpMeasurement = inMeasurement - (H * vPreState);

	// Compare prediction covariance with error
	double mCmpCovariance = (H * mPreCovariance * HT) + R;

	// Compute the Kalman gain
	double mKalman = mPreCovariance * HT / mCmpCovariance;

	// Update the state
	vState = vPreState + (mKalman * vCmpMeasurement);

	// Update the covariance
	mCovariance = (I - mKalman * H) * mPreCovariance;

	return vState;
}
