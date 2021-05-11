#include "UwbAdaptor.h"
#include "UwbLogging.h"
#include "LunaUwbServiceUtil.h"

UwbAdaptor* UwbAdaptor::mUwbAdaptor = nullptr;
UwbSpecInfo* UwbAdaptor::mUwbSpecInfo = nullptr;

UwbAdaptor* UwbAdaptor::getInstance() {
    if(mUwbAdaptor == nullptr) mUwbAdaptor = new UwbAdaptor();
    return mUwbAdaptor;
}

UwbAdaptor::UwbAdaptor() {
    UWB_LOG_INFO("UwbAdaptor Constructor");
}

UwbAdaptor::~UwbAdaptor() {

}

bool UwbAdaptor::init(LSHandle *sh) {
    UWB_LOG_INFO("UwbAdaptor Init");
    mLSHandle = sh;

    return true;
}

bool UwbAdaptor::getUwbServiceState(LSHandle *sh, LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getRangingInfo");

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
    writeServiceState(responseObj, m_isServiceAvailable);
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
    UWB_LOG_INFO("UwbAdaptor::getRangingInfo");
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
    writeSpecificInfo(responseObj, *mUwbSpecInfo);

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

    if(!mUwbRangingInfo) {
        mUwbRangingInfo = new UwbRangingInfo(m_connectionStatus, 1, "01", "Success", 30, 100);
    }
    writeRangingInfo(responseObj, *mUwbRangingInfo);

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

void UwbAdaptor::notifySubscriberServiceState(bool isServiceAvailable) {
    UWB_LOG_INFO("notifySubscriberServiceState");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();
    writeServiceState(responseObj, isServiceAvailable );
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

    writeSpecificInfo(responseObj, info);
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

void UwbAdaptor::notifySubscriberRangingInfo(UwbRangingInfo& rangingInfo)
{
    UWB_LOG_INFO("notifySubscriberRangingInfo");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();
    writeRangingInfo(responseObj, rangingInfo);
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

void UwbAdaptor::writeServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable) {
    UWB_LOG_INFO("writeServiceState");
    responseObj.put("serviceAvailability", isServiceAvailable);
}

void UwbAdaptor::writeSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info) {
    UWB_LOG_INFO("writeSpecificInfo");

    responseObj.put("modState",info.getModState());
    responseObj.put("fwVersion", info.getFwVersion());
    responseObj.put("fwCrc", info.getFwCrc());
}

void UwbAdaptor::writeRangingInfo(pbnjson::JValue &responseObj, UwbRangingInfo& rangingInfo) {
    UWB_LOG_INFO("writeRangingInfo");

    pbnjson::JValue rangingInfoArray = pbnjson::Array();
    pbnjson::JValue rangingInfoLgeAirCondObj = pbnjson::Object();

    pbnjson::JValue lgeAirCondArray = pbnjson::Array();
    pbnjson::JValue lgeAirCondData = pbnjson::Object();

    pbnjson::JValue lgeAirCondRangingData = pbnjson::Object();
    pbnjson::JValue lgeAirCondRangingArray = pbnjson::Array();

    responseObj.put("sessionId", m_sessionId);
    responseObj.put("rangingInfo", rangingInfoArray);

    rangingInfoArray.append(rangingInfoLgeAirCondObj);
    rangingInfoLgeAirCondObj.put("rangingDataLgeAirCond", lgeAirCondArray);

    lgeAirCondArray.append(lgeAirCondData);
    lgeAirCondData.put("remoteDeviceAddress", rangingInfo.getRemoteDevAddr());
    lgeAirCondData.put("connectionStatus", rangingInfo.getConnectionStatus());
    lgeAirCondData.put("condition", rangingInfo.getCondition());
    lgeAirCondData.put("receivedData", lgeAirCondRangingArray);

    lgeAirCondRangingArray.append(lgeAirCondRangingData);
    lgeAirCondRangingData.put("status", rangingInfo.getData()->getStatus());
    lgeAirCondRangingData.put("distance", rangingInfo.getData()->getDistance());
    lgeAirCondRangingData.put("angle", rangingInfo.getData()->getAngle());
}

void UwbAdaptor::updateServiceState(bool isServiceAvailable) {
    m_isServiceAvailable = isServiceAvailable;
    notifySubscriberServiceState(m_isServiceAvailable);
}
void UwbAdaptor::updateSpecificInfo(bool modState, string fwVersion, string fwCrc) {
    UwbSpecInfo *info = UwbSpecInfo::getInstance();
    info->setModState(modState);
    info->setFwVersion(fwVersion);
    info->setFwCrc(fwCrc);
    notifySubscriberSpecificInfo(*info);
}

void UwbAdaptor::updateRangingInfo(uint8_t condition, string remoteDevAddr, int64_t angle, int64_t distance) {
    UwbRangingInfo *rangingInfo = new UwbRangingInfo();

    rangingInfo->setRemoteDevAddr(remoteDevAddr);
    rangingInfo->setCondition(condition);

    rangingInfo->getData()->setDistance(distance);
    rangingInfo->getData()->setAngle(angle);

    if(!m_connectionStatus) {
        rangingInfo->setConnectionStatus(false);
        rangingInfo->getData()->setStatus("OutOfRange");
    } else {
        rangingInfo->setConnectionStatus(true);
        rangingInfo->getData()->setStatus("Success");

    }
    m_sessionId++;
    mUwbRangingInfo = rangingInfo;
    notifySubscriberRangingInfo(*rangingInfo);
    delete rangingInfo;
}

void UwbAdaptor::updateDisconnectedDevice(uint16_t deviceID) {
    m_connectionStatus = isDisconnected;
    m_sessionId = 0;
}
