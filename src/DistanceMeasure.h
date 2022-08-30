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

#ifndef _DISTANCEMEASURE_H
#define _DISTANCEMEASURE_H

class DistanceMeasure {
private:
    double mMeters;
    double mErrorMeters;
    double mConfidenceLevel;

public:
    DistanceMeasure();
    DistanceMeasure(double meters, double errorMeters, double confidenceLevel);
    ~DistanceMeasure() = default;

    double getMeters() const;
    double getErrorMeters() const;
    double getConfidenceLevel() const;

    void setMeters(double meters);
    void setErrorMeters(double errorMeters);
    void setConfidenceLevel(double confidenceLevel);

};

#endif