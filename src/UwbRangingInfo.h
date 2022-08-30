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

#ifndef _UWBRANGINGINFO_H
#define _UWBRANGINGINFO_H

#include <cstdint>
#include <string>
#include <memory>

#include "DistanceMeasure.h"
#include "AngleMeasure.h"

using namespace std;

class UwbRangingInfo {
public:

    UwbRangingInfo();
    ~UwbRangingInfo() = default;

    uint8_t getSessionId() const;
    bool getConnectionStatus() const;
    int getCondition() const;
    string getRemoteDevAddr() const;
    int64_t getElapsedTime() const;
    std::unique_ptr<DistanceMeasure>& getDistanceMeasure();
    std::unique_ptr<AngleMeasure>& getAzimuthAngleMeasure();
    std::unique_ptr<AngleMeasure>& getAltitudeAngleMeasure();
    void setSessionId(uint8_t sessionId);
    void setConnectionStatus(bool connectionStatus);
    void setCondition(int condition);
    void setRemoteDevAddr(string remoteDevAddr);
    void setElapsedTime(int64_t elapsedTime);
    void setDistanceMeasure(std::unique_ptr<DistanceMeasure> distanceMeasure);
    void setAzimuthAngleMeasure(std::unique_ptr<AngleMeasure> angleMeasure);
    void setAltitudeAngleMeasure(std::unique_ptr<AngleMeasure> angleMeasure);

private:
    uint8_t m_sessionId{0};
    int m_condition{0};
    string m_remoteDevAddr{""};
    int64_t mElapsedTime{0};
    bool m_connectionStatus{true};
    std::unique_ptr<DistanceMeasure> mDistanceMeasure;
    std::unique_ptr<AngleMeasure> mAzimuthAngleMeasure;
    std::unique_ptr<AngleMeasure> mAltitudeAngleMeasure;
};
#endif