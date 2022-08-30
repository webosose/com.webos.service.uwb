// @@@LICENSE
//
//      Copyright (c) 2022 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

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