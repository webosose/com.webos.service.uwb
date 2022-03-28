#include "UwbAdaptor.h"
#include "UwbLogging.h"
#include "LunaUwbServiceUtil.h"
#include "UwbResponseBuilder.h"

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

    if(!mSavedUwbRangingInfo) {    
        mSavedUwbRangingInfo = std::make_unique<UwbRangingInfo>();
    }

    std::unique_ptr<IResponseBuilder> responseBuilder = std::make_unique<UwbResponseBuilder>();
    responseBuilder->buildRangingInfo(responseObj, mSavedUwbRangingInfo);
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

void UwbAdaptor::notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo>& rangingInfo)
{
    UWB_LOG_INFO("notifySubscriberRangingInfo");
    LSError lserror;
    LSErrorInit(&lserror);

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("sessionId", m_sessionId);
    std::unique_ptr<IResponseBuilder> responseBuilder = std::make_unique<UwbResponseBuilder>();
    responseBuilder->buildRangingInfo(responseObj, rangingInfo);
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
