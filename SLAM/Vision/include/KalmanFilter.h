#pragma once

// Translated from https://github.com/ScottSWu/KalmanFilter/blob/master/kf.js

#include <armadillo.hpp>

using namespace arma;

class KalmanFilter {
protected:
	vec vState;
	mat mCovariance;
	mat mTransition;
	mat mObservation;
	mat mControl;
	mat mProcess;
	mat mMeasurement;
public:
	/**
	 * Create a new Kalman Filter with the given parameters.
	 *
	 * @param initialState      A vector representing the initial state (x0)
	 * @param initialCovariance A matrix representing the initial covariance (P)
	 * @param transitionMatrix  A matrix representing the transition of states (A)
	 * @param observationMatrix A matrix transforming measurements into states (H)
	 * @param controlMatrix     A matrix representing linear relationships in the
	 *                              control vector (B)
	 * @param processCovariance
	 *                  A matrix representing the process covariance (Q)
	 * @param measurementCovariance
	 *                  A matrix representing the measurement error covariance (R)
	 */
	KalmanFilter(vec inInitialState, mat inInitialCovariance, mat inTransitionMatrix, mat inObservationMatrix, mat inControlMatrix, mat inProcessCovariance, mat inMeasurementCovariance);
	~KalmanFilter();

	/**
	 * Update the filter.
	 *
	 * @param measurement       A vector of the current measurement
	 * @param control           A vector of the measurement controls
	 * 
	 * @return		The updated filtered state
	 */
	vec update(vec inMeasurement, vec inControl);
};
