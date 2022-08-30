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

#include "UwbRangingInfo.h"

UwbRangingInfo::UwbRangingInfo() : m_sessionId(0),
                                   m_condition(0),
                                   m_remoteDevAddr("NA"),
                                   mElapsedTime(0),
                                   m_connectionStatus(false) {
    mDistanceMeasure = std::make_unique<DistanceMeasure>(0.0, 0.0, 0.0);
    mAzimuthAngleMeasure = std::make_unique<AngleMeasure>(0.0, 0.0, 0.0);
    mAltitudeAngleMeasure = std::make_unique<AngleMeasure>(0.0, 0.0, 0.0);
}

uint8_t UwbRangingInfo::getSessionId() const{
    return m_sessionId;
}

bool UwbRangingInfo::getConnectionStatus() const{
    return m_connectionStatus;
}

int UwbRangingInfo::getCondition() const{
    return m_condition;
}

string UwbRangingInfo::getRemoteDevAddr() const{
    return m_remoteDevAddr;
}

int64_t UwbRangingInfo::getElapsedTime() const{
    return mElapsedTime;
}

std::unique_ptr<DistanceMeasure>& UwbRangingInfo::getDistanceMeasure(){
    return mDistanceMeasure;
}

std::unique_ptr<AngleMeasure>& UwbRangingInfo::getAzimuthAngleMeasure(){
    return mAzimuthAngleMeasure;
}

std::unique_ptr<AngleMeasure>& UwbRangingInfo::getAltitudeAngleMeasure(){
    return mAltitudeAngleMeasure;
}

void UwbRangingInfo::setSessionId(uint8_t sessionId) {
    m_sessionId = sessionId;
}

void UwbRangingInfo::setConnectionStatus(bool connectionStatus) {
    m_connectionStatus = connectionStatus;
}

void UwbRangingInfo::setCondition(int condition) {
    m_condition = condition;
}

void UwbRangingInfo::setRemoteDevAddr(string remoteDevAddr) {
     m_remoteDevAddr = remoteDevAddr;
}

void UwbRangingInfo::setElapsedTime(int64_t elapsedTime) {
    mElapsedTime = elapsedTime;
}

void UwbRangingInfo::setDistanceMeasure(std::unique_ptr<DistanceMeasure> distanceMeasure) {
    mDistanceMeasure = std::move(distanceMeasure);
}

void UwbRangingInfo::setAzimuthAngleMeasure(std::unique_ptr<AngleMeasure> angleMeasure) {
    mAzimuthAngleMeasure = std::move(angleMeasure);
}

void UwbRangingInfo::setAltitudeAngleMeasure(std::unique_ptr<AngleMeasure> angleMeasure) {
    mAltitudeAngleMeasure = std::move(angleMeasure);
}
