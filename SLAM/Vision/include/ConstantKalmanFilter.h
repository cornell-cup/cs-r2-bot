#pragma once

/**
 * Class for a independent, single variable, constant state Kalman Filter.
 */
class ConstantKalmanFilter {
protected:
	double vState;
	double mCovariance;
	double mTransition = 1.0;
	double mObservation = 1.0;
	double mControl = 1.0;
	double mProcess;
	double mMeasurement;
public:
    /**
	 * Create a new single variable, constant process Kalman Filter with the given parameters.
	 *
	 * @param initialState      A vector representing the initial state (x0)
	 * @param initialCovariance A matrix representing the initial covariance (P)
	 * @param processCovariance
	 *                  A matrix representing the process covariance (Q)
	 * @param measurementCovariance
	 *                  A matrix representing the measurement error covariance (R)
	 */
	ConstantKalmanFilter(double inInitialState, double inInitialCovariance, double inProcessCovariance, double inMeasurementCovariance);
	~ConstantKalmanFilter();

	double update(double measurement);
};
