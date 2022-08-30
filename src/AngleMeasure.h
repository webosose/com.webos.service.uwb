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

#ifndef _ANGLEMEASURE_H
#define _ANGLEMEASURE_H

class AngleMeasure {
private:
    double mRadians;
    double mErrorRadians;
    double mConfidenceLevel;

public:
    AngleMeasure();
    AngleMeasure(double radians, double errorRadians, double confidenceLevel);
    ~AngleMeasure() = default;

    double getRadians() const;
    double getErrorRadians() const;
    double getConfidenceLevel() const;

    void setRadians(double radians);
    void setErrorRadians(double errorRadians);
    void setConfidenceLevel(double confidenceLevel);
};

#endif