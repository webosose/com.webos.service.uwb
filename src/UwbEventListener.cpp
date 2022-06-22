#include "UwbEventListener.h"

using namespace std::chrono;

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

void UwbEventListener::updateRangingInfo(uint8_t condition, uint8_t sessionId, int64_t angle, int64_t distance) {
    UWB_LOG_DEBUG("updateRangingInfo : condition [%d], sessionId [%d], angle [%lld], distance [%lld]", condition, sessionId, angle, distance);
    double distanceMeter = (double)distance / 100;
    double angleRadians = (double)(angle * 3.14) / 180;
    
    auto rangingInfo = std::make_unique<UwbRangingInfo>();
    auto distanceMeasure = std::make_unique<DistanceMeasure>(distanceMeter, 1.0, condition);
    auto angleMeasure = std::make_unique<AngleMeasure>(angleRadians, 1.0, condition);
    //TODO: Add altitudeAngle measurement.
    
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());    
    rangingInfo->setDistanceMeasure(std::move(distanceMeasure));
    rangingInfo->setAzimuthAngleMeasure(std::move(angleMeasure));
    rangingInfo->setElapsedTime(ms.count());
    rangingInfo->setRemoteDevAddr("01");
    rangingInfo->setCondition(condition);
    rangingInfo->setConnectionStatus(true);
    rangingInfo->setSessionId(sessionId);
    
    UwbServiceManager::getInstance()->notifySubscriberRangingInfo(std::move(rangingInfo), sessionId);
}