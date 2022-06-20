#include "UwbAdaptor.h"
#include "UwbLogging.h"
#include "LunaUwbServiceUtil.h"

UwbSpecInfo* UwbAdaptor::mUwbSpecInfo = nullptr;

UwbAdaptor::UwbAdaptor():mResponseBuilder(std::make_unique<UwbResponseBuilder>()) {
    UWB_LOG_INFO("UwbAdaptor Constructor");
}

UwbAdaptor::~UwbAdaptor() {

}

bool UwbAdaptor::init(LSHandle *sh) {
    UWB_LOG_INFO("UwbAdaptor Init");
    mLSHandle = sh;

    return true;
}

void UwbAdaptor::setDeviceInterface(std::shared_ptr<UartSerial> uartSerial) {
    mUartSerial = uartSerial;
    mUartSerial->setAdaptor(shared_from_this());
}

bool UwbAdaptor::getUwbServiceState(LSHandle *sh, LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getUwbServiceState");

    LSError mLSError;
    bool isSubscription = false;
    pbnjson::JValue responseObj = pbnjson::Object();

    LSErrorInit(&mLSError);

    if (LSMessageIsSubscription(message)) {
        isSubscription = true;
        if (LSSubscriptionAdd(sh, "getUwbServiceState", message, &mLSError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &mLSError );
            return true;
        }
    }

    if (responseObj.isNull())
            return false;
    mResponseBuilder->buildServiceState(responseObj, m_isServiceAvailable);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", isSubscription);

    if (!LSMessageReply(sh, message, responseObj.stringify().c_str(), &mLSError))
    {
        UWB_LOG_ERROR("HANDLE_getUwbServiceState Message reply error!!");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }
    return true;
}

bool UwbAdaptor::getUwbSpecificInfo(LSHandle *sh, LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getUwbSpecificInfo");
    LSError mLSError;
    bool isSubscription = false;
    pbnjson::JValue responseObj = pbnjson::Object();

    LSErrorInit(&mLSError);

    if (LSMessageIsSubscription(message)) {
        isSubscription = true;
        if (LSSubscriptionAdd(sh, "getUwbSpecificInfo", message, &mLSError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &mLSError );
            return true;
        }
    }

    if (responseObj.isNull())
            return false;

    mUwbSpecInfo = UwbSpecInfo::getInstance();
    mResponseBuilder->buildSpecificInfo(responseObj, *mUwbSpecInfo);

    responseObj.put("returnValue", true);
    responseObj.put("subscribed", isSubscription);

    if (!LSMessageReply(sh, message, responseObj.stringify().c_str(), &mLSError))
    {
        UWB_LOG_ERROR("HANDLE_getUwbServiceState Message reply error!!");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }

    return true;
}

bool UwbAdaptor::getRangingInfo(LSHandle *sh, LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getRangingInfo");
    LSError mLSError;
    bool isSubscription = false;
    pbnjson::JValue responseObj = pbnjson::Object();

    LSErrorInit(&mLSError);

    if (LSMessageIsSubscription(message)) {
        isSubscription = true;
        if (LSSubscriptionAdd(sh, "getRangingInfo", message, &mLSError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &mLSError );
            return true;
        }
    }

    if (responseObj.isNull())
        return false;

    if(!mSavedUwbRangingInfo) {    
        mSavedUwbRangingInfo = std::make_unique<UwbRangingInfo>();
    }

    mResponseBuilder->buildRangingInfo(responseObj, mSavedUwbRangingInfo);
    responseObj.put("sessionId", m_sessionId);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", isSubscription);

    if (!LSMessageReply(sh, message, responseObj.stringify().c_str(), &mLSError))
    {
        UWB_LOG_ERROR("HANDLE_getUwbServiceState Message reply error!!");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }

    return true;
}

UwbErrorCodes UwbAdaptor::startDiscovery(int32_t discoveryTimeout) {
    UWB_LOG_INFO("UwbAdaptor::startDiscovery");
    UwbErrorCodes error = UWB_ERROR_NONE;
    //TODO: Add call to driver API to start discovery
    return error;
}

bool UwbAdaptor::stopDiscovery(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::stopDiscovery");
    //TODO: Add call to driver API
    return true;
}

UwbErrorCodes UwbAdaptor::setUwbModuleState(const std::string& moduleState) {
    UWB_LOG_INFO("UwbAdaptor::setUwbModuleState");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(moduleState == "start") {
        error = mUartSerial->setUwbModuleState(HOST_CMD_MODULE_START);
    }
    else if(moduleState == "stop") {
        error = mUartSerial->setUwbModuleState(HOST_CMD_MODULE_STOP);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

UwbErrorCodes UwbAdaptor::getStatus() {
    UWB_LOG_INFO("UwbAdaptor::getStatus");

    UwbErrorCodes error = UWB_ERROR_NONE;
    error = mUartSerial->getUwbModuleInfo();
    return error;
}

bool UwbAdaptor::getPairedSessions(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getPairedSessions");
    //TODO: Add call to driver API
    return true;
}

UwbErrorCodes UwbAdaptor::setDeviceType(const std::string& deviceType) {
    UWB_LOG_INFO("UwbAdaptor::setState");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(deviceType == "controller") {
        error = mUartSerial->setDeviceType(0);
    }
    else if(deviceType == "controlee") {
        error = mUartSerial->setDeviceType(1);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

UwbErrorCodes UwbAdaptor::setDeviceName(const std::string& deviceName) {
    UWB_LOG_INFO("UwbAdaptor::setDeviceName");
    UwbErrorCodes error = UWB_ERROR_NONE;
    error = mUartSerial->setDeviceName(deviceName);
    return error;
}

UwbErrorCodes UwbAdaptor::setDeviceMode(const std::string& deviceMode) {
    UWB_LOG_INFO("UwbAdaptor::setDeviceMode");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(deviceMode == "ranging") {
        error = mUartSerial->setDeviceMode(0);
    }
    else if(deviceMode == "dataTransfer") {
        error = mUartSerial->setDeviceMode(1);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

bool UwbAdaptor::openSession(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::openSession");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::closeSession(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::closeSession");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::startRanging(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::startRanging");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::stopRanging(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::stopRanging");
    //TODO: Add call to driver API
    return true;
}

void UwbAdaptor::notifySubscriberServiceState(bool isServiceAvailable) {
    UWB_LOG_INFO("notifySubscriberServiceState");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();
    mResponseBuilder->buildServiceState(responseObj, isServiceAvailable);
    responseObj.put("subscribed", true);
    responseObj.put("returnValue", true);

    if (!LSSubscriptionReply(mLSHandle, "getUwbServiceState", responseObj.stringify().c_str(), &lserror))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lserror);
        return;
    }
    LSErrorFree(&lserror);
    return;
}

void UwbAdaptor::notifySubscriberSpecificInfo(UwbSpecInfo& info) {
    UWB_LOG_INFO("notifySubscriberSpecificInfo");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();

    mResponseBuilder->buildSpecificInfo(responseObj, info);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);

    if (!LSSubscriptionReply(mLSHandle, "getUwbSpecificInfo", responseObj.stringify().c_str(), &lserror))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lserror);
        return;
    }
    LSErrorFree(&lserror);
    return;
}

void UwbAdaptor::notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo>& rangingInfo)
{
    UWB_LOG_INFO("notifySubscriberRangingInfo");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("sessionId", m_sessionId);
    mResponseBuilder->buildRangingInfo(responseObj, rangingInfo);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);

    if (!LSSubscriptionReply(mLSHandle, "getRangingInfo", responseObj.stringify().c_str(), &lserror))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lserror);
        return;
    }

    LSErrorFree(&lserror);
    return;
}

void UwbAdaptor::updateServiceState(bool isServiceAvailable) {
    m_isServiceAvailable = isServiceAvailable;
    notifySubscriberServiceState(m_isServiceAvailable);
}
void UwbAdaptor::updateSpecificInfo(bool modState, string fwVersion, string fwCrc) {
    bool cModeState = !(modState); // 0 -> available in UART Protocol
    UWB_LOG_INFO("updateSpecificInfo : modState [%d], fwVersion [%s], fwCrc [%s]", cModeState, fwVersion.c_str(), fwCrc.c_str());

    UwbSpecInfo *info = UwbSpecInfo::getInstance();
    info->setModState(cModeState);
    info->setFwVersion(fwVersion);
    info->setFwCrc(fwCrc);
    notifySubscriberSpecificInfo(*info);
}

void UwbAdaptor::updateRangingInfo(int condition, string remoteDevAddr, int64_t angle, int64_t distance) {
    UWB_LOG_INFO("updateRangingInfo : condition [%d], remoteDevAddr [%s], angle [%lld], distance [%lld]", condition, remoteDevAddr.c_str(), angle, distance);
    //TEMP code, service state = true
    updateServiceState(true);

    auto rangingInfo = std::make_unique<UwbRangingInfo>();
    auto distanceMeasure = std::make_unique<DistanceMeasure>(distance, 1.0, 1.0);
    auto angleMeasure = std::make_unique<AngleMeasure>(angle, 1.0, 1.0);
    //TODO: Add altitudeAngle measurement.
    rangingInfo->setDistanceMeasure(std::move(distanceMeasure));
    rangingInfo->setAzimuthAngleMeasure(std::move(angleMeasure));
    rangingInfo->setElapsedTime(1000);

    rangingInfo->setRemoteDevAddr(remoteDevAddr);
    rangingInfo->setCondition(condition);

    //If this API is called, it means that the UWB module is working well and also ranging is successfully received.
    //TEMP code, to trigger connectionStatus
    rangingInfo->setConnectionStatus(true);

    m_sessionId++;
    rangingInfo->setSessionId(m_sessionId);

    notifySubscriberRangingInfo(rangingInfo);
    mSavedUwbRangingInfo = std::move(rangingInfo);
}

void UwbAdaptor::updateDisconnectedDevice(uint16_t deviceID) {
    UWB_LOG_INFO("updateDisconnectedStatus : deviceID [%d]", deviceID);

    //TEMP code, service state = true
    updateServiceState(false);

    //If this API is called, it means that the UWB Tag (DeviceID) is disconnected and also ranging can be not received.
    //TEMP code, to trigger connectionStatus
    //UwbRangingInfo *rangingInfo = new UwbRangingInfo();

    auto disConnRangingInfo = std::make_unique<UwbRangingInfo>();
    disConnRangingInfo->setConnectionStatus(false);
    disConnRangingInfo->setCondition(255); // 255 : invalid ranging info, (0 : valid)
    m_sessionId = 0;
    disConnRangingInfo->setSessionId(m_sessionId);

    notifySubscriberRangingInfo(disConnRangingInfo);
    mSavedUwbRangingInfo = std::move(disConnRangingInfo); // for saving up-to-date ranging data

    //m_connectionStatus = isDisconnected;
}

void UwbAdaptor::updateModuleStateChanged(const std::string& moduleState){
    UwbServiceManager::getInstance()->notifyModuleStateChanged(moduleState);
}

void UwbAdaptor::updateDeviceTypeChanged(uint8_t deviceType) {
    if(deviceType == 0) {
        UwbServiceManager::getInstance()->notifyDeviceRoleChanged("controller");
    }
    else if (deviceType == 1) {
        UwbServiceManager::getInstance()->notifyDeviceRoleChanged("controlee");
    }
}

void UwbAdaptor::updateDeviceModeChanged(uint8_t deviceMode) {
    if(deviceMode == 0) {
        UwbServiceManager::getInstance()->notifyDeviceModeChanged("ranging");
    }
    else if (deviceMode == 1) {
        UwbServiceManager::getInstance()->notifyDeviceModeChanged("dataTransfer");
    }
}

void UwbAdaptor::updateDeviceNameChanged(const std::string& deviceName) {
    UwbServiceManager::getInstance()->notifyDeviceNameChanged(deviceName);
}
