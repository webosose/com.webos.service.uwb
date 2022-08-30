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

#include "UwbEventListener.h"

using namespace std::chrono;

void UwbEventListener::updateModuleStatus() {
    UwbServiceManager::getInstance()->notifySubscribersModuleStatus();
}

void UwbEventListener::updateModuleStateChanged(const std::string& moduleState){
    UwbServiceManager::getInstance()->notifyModuleStateChanged(moduleState);
}

void UwbEventListener::updateDeviceTypeChanged(uint8_t deviceType) {
    if(deviceType == 0) {
        UwbServiceManager::getInstance()->notifyDeviceRoleChanged("controller");
    }
    else if (deviceType == 1) {
        UwbServiceManager::getInstance()->notifyDeviceRoleChanged("controlee");
    }
}

void UwbEventListener::updateDeviceModeChanged(uint8_t deviceMode) {
    if(deviceMode == 0) {
        UwbServiceManager::getInstance()->notifyDeviceModeChanged("ranging");
    }
    else if (deviceMode == 1) {
        UwbServiceManager::getInstance()->notifyDeviceModeChanged("dataTransfer");
    }
}

void UwbEventListener::updateDeviceNameChanged(const std::string& deviceName) {
    UwbServiceManager::getInstance()->notifyDeviceNameChanged(deviceName);
}

void UwbEventListener::updatePairingFlag(bool pairingFlag) {
    UwbServiceManager::getInstance()->notifyPairingFlagChanged(pairingFlag);
}

void UwbEventListener::updatePairingInfo() {
    UwbServiceManager::getInstance()->notifyPairingInfo();
}

void UwbEventListener::updateDiscoveryStatus(bool discoveryStatus) {
    UwbServiceManager::getInstance()->notifyDiscoveryStatus(discoveryStatus);
}

void UwbEventListener::updateScanResult(const std::string& macAddress, const std::string& deviceName) {
    UwbServiceManager::getInstance()->notifyScanResult(macAddress, deviceName);
}

void UwbEventListener::updateOpenSessionResponse(uint8_t sessionId) {
    UwbServiceManager::getInstance()->notifyOpenSessionResponse(sessionId);
}

void UwbEventListener::updateRangingInfo(uint8_t reliability, uint8_t sessionId, int64_t angle, int64_t distance) {
    UWB_LOG_DEBUG("updateRangingInfo : reliability [%d], sessionId [%d], angle [%lld], distance [%lld]", reliability, sessionId, angle, distance);
    double distanceMeter = (double)distance / 100;
    double angleRadians = (double)(angle * 3.14) / 180;

    auto rangingInfo = std::make_unique<UwbRangingInfo>();
    auto distanceMeasure = std::make_unique<DistanceMeasure>(distanceMeter, 1.0, reliability);
    auto angleMeasure = std::make_unique<AngleMeasure>(angleRadians, 1.0, reliability);
    //TODO: Add altitudeAngle measurement.

    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    rangingInfo->setDistanceMeasure(std::move(distanceMeasure));
    rangingInfo->setAzimuthAngleMeasure(std::move(angleMeasure));
    rangingInfo->setElapsedTime(ms.count());
    rangingInfo->setRemoteDevAddr("01");
    rangingInfo->setCondition(reliability);
    rangingInfo->setConnectionStatus(true);
    rangingInfo->setSessionId(sessionId);

    UwbServiceManager::getInstance()->notifySubscriberRangingInfo(std::move(rangingInfo), sessionId);
}