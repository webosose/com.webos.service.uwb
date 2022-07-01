#include "AngleMeasure.h"


AngleMeasure::AngleMeasure():mRadians(0.0), mErrorRadians(0.0), mConfidenceLevel(0.0){
	
}

AngleMeasure::AngleMeasure(double radians, double errorRadians, double confidenceLevel):
mRadians(radians), mErrorRadians(errorRadians), mConfidenceLevel(confidenceLevel) {
	
}

double AngleMeasure::getRadians() const{
	return mRadians;
}

double AngleMeasure::getErrorRadians() const{
	return mErrorRadians;
}

double AngleMeasure::getConfidenceLevel() const{
	return mConfidenceLevel;
}

void AngleMeasure::setRadians(double radians){
	mRadians = radians;
}

void AngleMeasure::setErrorRadians(double errorRadians){
	mErrorRadians = errorRadians;
}

void AngleMeasure::setConfidenceLevel(double confidenceLevel){
	mConfidenceLevel = confidenceLevel;
}