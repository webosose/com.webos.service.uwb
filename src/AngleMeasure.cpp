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