#include "DistanceMeasure.h"

DistanceMeasure::DistanceMeasure():mMeters(0.0), mErrorMeters(0.0), mConfidenceLevel(false){
	
}

DistanceMeasure::DistanceMeasure(double meters, double errorMeters, double confidenceLevel):mMeters(meters), mErrorMeters(errorMeters), mConfidenceLevel(confidenceLevel){
	
}

double DistanceMeasure::getMeters() const{
	return mMeters;
}

double DistanceMeasure::getErrorMeters() const{
	return mErrorMeters;
}

double DistanceMeasure::getConfidenceLevel() const{
	return mConfidenceLevel;
}

void DistanceMeasure::setMeters(double meters){
	mMeters = meters;
}

void DistanceMeasure::setErrorMeters(double errorMeters){
	mErrorMeters = errorMeters;
}

void DistanceMeasure::setConfidenceLevel(double confidenceLevel){
	mConfidenceLevel = confidenceLevel;
}