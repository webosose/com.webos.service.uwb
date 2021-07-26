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
    delete mSavedUwbRangingInfo;
    delete mRangingInfo;
    delete disConnRangingInfo;
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
        mSavedUwbRangingInfo = new UwbRangingInfo(m_connectionStatus, 1, "01", "Success", 30, 100);
    }
    writeRangingInfo(responseObj, *mSavedUwbRangingInfo);

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

    UWB_LOG_INFO("writeRangingInfo : rangingInfo.getConnectionStatus() = [%d]", (int)(rangingInfo.getConnectionStatus()));
    int comp_connStatus = (int)(rangingInfo.getConnectionStatus());
    UWB_LOG_INFO("writeRangingInfo : comp_connStatus = [%d]", (int)(comp_connStatus));

    if( comp_connStatus == 0 ) {
        lgeAirCondData.put("connectionStatus", false);
    }
    else {
        lgeAirCondData.put("connectionStatus", true);
    }

    UWB_LOG_INFO("writeRangingInfo : rangingInfo.getCondition() = [%d]", rangingInfo.getCondition());
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

    //UwbRangingInfo *rangingInfo = new UwbRangingInfo();
    mRangingInfo = new UwbRangingInfo();

    mRangingInfo->setRemoteDevAddr(remoteDevAddr);
    mRangingInfo->setCondition(condition);

    mRangingInfo->getData()->setDistance(distance);
    mRangingInfo->getData()->setAngle(angle);

    //If this API is called, it means that the UWB module is working well and also ranging is successfully received.
    //TEMP code, to trigger connectionStatus
    mRangingInfo->setConnectionStatus(true);
    mRangingInfo->getData()->setStatus("Success");

    m_sessionId++;
    mSavedUwbRangingInfo = mRangingInfo;
    notifySubscriberRangingInfo(*mRangingInfo);

    //Need to single tone obj and remove the below line
    //delete mRangingInfo;
}

void UwbAdaptor::updateDisconnectedDevice(uint16_t deviceID) {
    UWB_LOG_INFO("updateDisconnectedStatus : deviceID [%d]", deviceID);

    //TEMP code, service state = true
    updateServiceState(false);

    //If this API is called, it means that the UWB Tag (DeviceID) is disconnected and also ranging can be not received.
    //TEMP code, to trigger connectionStatus
    //UwbRangingInfo *rangingInfo = new UwbRangingInfo();
    disConnRangingInfo = new UwbRangingInfo();
    disConnRangingInfo->setConnectionStatus(false);
    disConnRangingInfo->getData()->setStatus("OutOfRange");
    disConnRangingInfo->setCondition(255); // 255 : invalid ranging info, (0 : valid)
    mSavedUwbRangingInfo = disConnRangingInfo; // for saving up-to-date ranging data
    notifySubscriberRangingInfo(*disConnRangingInfo);
    //Need to single tone obj and remove the below line
    //delete rangingInfo;

    //m_connectionStatus = isDisconnected;
    m_sessionId = 0;
}
