#include "stdafx.h"
#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(vec inInitialState, mat inInitialCovariance, mat inTransitionMatrix, mat inObservationMatrix, mat inControlMatrix, mat inProcessCovariance, mat inMeasurementCovariance) : vState(inInitialState), mCovariance(inInitialCovariance), mTransition(inTransitionMatrix), mControl(inControlMatrix), mProcess(inProcessCovariance), mMeasurement(inMeasurementCovariance) {

}

KalmanFilter::~KalmanFilter() {

}

vec KalmanFilter::update(vec inMeasurement, vec inControl) {
	// Short names
	vec xp = vState;
	mat pp = mCovariance;

	mat A = mTransition;
	mat AT = A.t();
	mat B = mControl;
	mat H = mObservation;
	mat HT = H.t();
	mat I = mat(pp.n_rows, pp.n_rows, fill::eye);
	mat Q = mProcess;
	mat R = mMeasurement;

	// Predict the next state
	vec vPreState = (A * xp) + (B * inControl);

	// Predict covariance
	mat mPreCovariance = (A * pp * AT) + Q;

	// Compare prediction with measurement
	vec vCmpMeasurement = inMeasurement - (H * vPreState);

	// Compare prediction covariance with error
	mat mCmpCovariance = (H * mPreCovariance * HT) + R;

	// Compute the Kalman gain
	mat mKalman = mPreCovariance * HT * (mCmpCovariance.i());

	// Update the state
	vState = vPreState + (mKalman * vCmpMeasurement);

	// Update the covariance
	mCovariance = (I - mKalman * H) * mPreCovariance;

	return vState;
}
