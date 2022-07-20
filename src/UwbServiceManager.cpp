#include "UwbServiceManager.h"
#include "LunaUwbServiceUtil.h"
#include <pbnjson.hpp>

LSMethod UwbServiceManager::serviceMethods[] = {
    { "getUwbServiceState", UwbServiceManager::_getUwbServiceState },
    { "getUwbSpecificInfo", UwbServiceManager::_getUwbSpecificInfo },
    { "getRangingInfo",     UwbServiceManager::_getRangingInfo },
    { "setUwbModuleState", UwbServiceManager::_setUwbModuleState },
    { "getStatus", UwbServiceManager::_getStatus },
    { "getPairedSessions",     UwbServiceManager::_getPairedSessions },
    { "setState",     UwbServiceManager::_setState },
    { "startDiscovery",     UwbServiceManager::_startDiscovery },
    { "stopDiscovery", UwbServiceManager::_stopDiscovery },
    { "openSession", UwbServiceManager::_openSession },
    { "closeSession",     UwbServiceManager::_closeSession },
    { "startRanging", UwbServiceManager::_startRanging },
    { "stopRanging", UwbServiceManager::_stopRanging },  
    { 0               ,      0                 }
};

UwbServiceManager *UwbServiceManager::getInstance() {
    if(mUwbServiceMgr == nullptr) mUwbServiceMgr = new UwbServiceManager();
    return mUwbServiceMgr;
}

UwbServiceManager::UwbServiceManager():
        mMainLoop(nullptr),
        mUwbSessionCtl(nullptr) {
}

UwbServiceManager::~UwbServiceManager(){
    delete mUwbSessionCtl;
}

bool UwbServiceManager::init(GMainLoop *mainLoop, std::shared_ptr<UwbAdapterInterface> adapter) {

    if (UwbServiceRegister(UWB_SERVICE_NAME, mainLoop, &mServiceHandle) == false) {
        UWB_LOG_ERROR("com.webos.service.uwb service registration failed");
        return false;
    }

    UWB_LOG_DEBUG("mServiceHandle =%p", mServiceHandle);
    mUwbAdaptor = adapter;

    UWB_LOG_INFO("uwbServiceManager this = %p", this );
    mUwbAdaptor->init(mServiceHandle);

    return true;
}

bool UwbServiceManager::UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle) {

    LSError mLSError;
    LSErrorInit(&mLSError);

    if(!LSRegister(srvcname, mServiceHandle, &mLSError))
    {
        UWB_LOG_ERROR("LS_REGISTER :Unable to register to luna-bus");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }

    if(!LSGmainAttach(*mServiceHandle, mainLoop, &mLSError))
    {
        UWB_LOG_ERROR("LS_REGISTER : unable to attach service");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }

    if (!LSRegisterCategory(*mServiceHandle, "/", serviceMethods, NULL, NULL, &mLSError))
    {
        UWB_LOG_ERROR("LS_REGISTER : Unable to register category and method");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }


    return true;
}

bool UwbServiceManager::deinit() {
    return true;
}

bool UwbServiceManager::getUwbServiceState(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    return true;
}

bool UwbServiceManager::getUwbSpecificInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
   
    return true;
}

bool UwbServiceManager::getRangingInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    
    LS::Message request(message);
    bool subscribed = false;
    pbnjson::JValue responseObj = pbnjson::Object();
    
    LSError lsError;
    LSErrorInit(&lsError);
    if (LSMessageIsSubscription(message)) {
        subscribed = true;
        if (LSSubscriptionAdd(sh, "getRangingInfo", message, &lsError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &lsError );
            return true;
        }
    }
   
    if(!mSavedUwbRangingInfo) {    
        mSavedUwbRangingInfo = std::make_unique<UwbRangingInfo>();
    }

    mResponseBuilder->buildRangingInfo(responseObj, mSavedUwbRangingInfo);

    responseObj.put("sessionId", 1); //TODO: Update logic when session support is added
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", subscribed);

    if (!LSMessageReply(sh, message, responseObj.stringify().c_str(), &lsError))
    {
        UWB_LOG_ERROR("HANDLE_getUwbServiceState Message reply error!!");
        LSErrorPrint(&lsError, stdout);

        return false;
    }
    
    return true;
}

void UwbServiceManager::notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo> rangingInfo, uint8_t sessionId)
{
    UWB_LOG_DEBUG("notifySubscriberRangingInfo");
    LSError lsError;
    LSErrorInit(&lsError);

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("sessionId", sessionId);
    mResponseBuilder->buildRangingInfo(responseObj, rangingInfo);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);
    mSavedUwbRangingInfo = std::move(rangingInfo);
    if (!LSSubscriptionReply(mServiceHandle, "getRangingInfo", responseObj.stringify().c_str(), &lsError))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lsError);
        return;
    }

    LSErrorFree(&lsError);   

    return;
}

bool UwbServiceManager::startDiscovery(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    pbnjson::JValue responseObj = pbnjson::Object();
    
    if(mModuleInfo.getDeviceRole() == "controlee") {
        LSUtils::respondWithError(request, UWB_ERROR_UNSUPPORTED_API_CONTROLEE);
        return true;
    }
    
	pbnjson::JValue requestObj;
	int parseError = 0;

    const std::string schema = STRICT_SCHEMA(PROPS_2(PROP(discoveryTimeout, integer), PROP(subscribe, boolean)) REQUIRED_1(subscribe));

    if(!LSUtils::parsePayload(request.getPayload(),requestObj,schema,&parseError))
	{
		if(parseError != JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		else if (!request.isSubscription())
			LSUtils::respondWithError(request, UWB_ERR_MTHD_NOT_SUBSCRIBED);
		else
			LSUtils::respondWithError(request,UWB_ERR_SCHEMA_VALIDATION_FAIL);

		return true;
	}

    int32_t discoveryTimeout = 0;
    if (requestObj.hasKey("discoveryTimeout"))
	{
		discoveryTimeout = requestObj["discoveryTimeout"].asNumber<int32_t>();
        if (discoveryTimeout < 0)
        {
                LSUtils::respondWithError(request, retrieveErrorText(UWB_ERR_DISCOVERY_TO_NEG_VALUE) + std::to_string(discoveryTimeout), UWB_ERR_DISCOVERY_TO_NEG_VALUE);
                return true;
        }
    }
    
    LSError lsError;
    bool isSubscription = false;
    
    if (request.isSubscription())
    {
        if (LSSubscriptionAdd(sh, "startDiscovery", message, &lsError) == false) {
            UWB_LOG_ERROR("Failed to add startDiscovery to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &lsError );
            return true;
        }
        isSubscription = true;
    }
    
    UwbErrorCodes error = UWB_ERROR_NONE;

    error = mUwbAdaptor->startDiscovery(discoveryTimeout);

    if (error != UWB_ERROR_NONE)
    {
        LSUtils::respondWithError(request, UWB_ERR_START_DISC_FAIL);
        return true;
    }
    
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", isSubscription);
    LSUtils::postToClient(request, responseObj);

    return true;
}

bool UwbServiceManager::setUwbModuleState(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
	pbnjson::JValue requestObj;
	int parseError = 0;

    const std::string schema = STRICT_SCHEMA(PROPS_1(PROP(moduleState, string)) REQUIRED_1(moduleState));

	if (!LSUtils::parsePayload(request.getPayload(), requestObj, schema, &parseError))
	{
		if (parseError == JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_SCHEMA_VALIDATION_FAIL);
		else
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		return true;
	}

    if (requestObj.hasKey("moduleState"))
	{
		std::string moduleState = requestObj["moduleState"].asString();

		UwbErrorCodes error = UWB_ERROR_NONE;
        UWB_LOG_INFO("setUwbModuleState : moduleState [%s]", moduleState.c_str());

        error = mUwbAdaptor->setUwbModuleState(moduleState);

		if (error != UWB_ERROR_NONE)
		{
			LSUtils::respondWithError(request, error);
			return true;
		}
	}

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    
    return true;
}

bool UwbServiceManager::getStatus(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    bool subscribed = false;
    pbnjson::JValue responseObj = pbnjson::Object();
    
    LSError lsError;
    LSErrorInit(&lsError);
    if (LSMessageIsSubscription(message)) {
        subscribed = true;
        if (LSSubscriptionAdd(sh, "getStatus", message, &lsError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &lsError );
            return true;
        }
    }
    
    UwbServiceManager::getInstance()->appendCurrentStatus(responseObj);
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", subscribed);

    LSUtils::postToClient(request, responseObj);
    return true;
}

bool UwbServiceManager::getPairedSessions(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->getPairedSessions(message);
    
    return true;
}

bool UwbServiceManager::setState(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
	pbnjson::JValue requestObj;
	int parseError = 0;

    const std::string schema = STRICT_SCHEMA(PROPS_3(PROP(role, string), PROP(deviceName, string), PROP(deviceMode, string)));

	if (!LSUtils::parsePayload(request.getPayload(), requestObj, schema, &parseError))
	{
		if (parseError == JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_SCHEMA_VALIDATION_FAIL);
		else
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		return true;
	}

    if (requestObj.hasKey("role"))
	{
		std::string role = requestObj["role"].asString();

		UwbErrorCodes error = UWB_ERROR_NONE;
        UWB_LOG_INFO("setState : role [%s]", role.c_str());

        error = mUwbAdaptor->setDeviceType(role);

		if (error != UWB_ERROR_NONE)
		{
			LSUtils::respondWithError(request, error);
			return true;
		}
        usleep(50000);
	}
    
    if (requestObj.hasKey("deviceName"))
	{
		std::string deviceName = requestObj["deviceName"].asString();

		UwbErrorCodes error = UWB_ERROR_NONE;
        UWB_LOG_INFO("setState : deviceName [%s]", deviceName.c_str());

        error = mUwbAdaptor->setDeviceName(deviceName);

		if (error != UWB_ERROR_NONE)
		{
			LSUtils::respondWithError(request, error);
			return true;
		}
        usleep(50000);
	}
    
    if (requestObj.hasKey("deviceMode"))
	{
		std::string deviceMode = requestObj["deviceMode"].asString();

		UwbErrorCodes error = UWB_ERROR_NONE;
        UWB_LOG_INFO("setState : deviceMode [%s]", deviceMode.c_str());

        error = mUwbAdaptor->setDeviceMode(deviceMode);

		if (error != UWB_ERROR_NONE)
		{
			LSUtils::respondWithError(request, error);
			return true;
		}
	}

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    
    return true;
}

bool UwbServiceManager::stopDiscovery(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);    
    
    if(mModuleInfo.getDeviceRole() == "controlee") {
        LSUtils::respondWithError(request, UWB_ERROR_UNSUPPORTED_API_CONTROLEE);
        return true;
    }
    
    UwbErrorCodes error = UWB_ERROR_NONE;
    error = mUwbAdaptor->stopDiscovery();
    
    if (error != UWB_ERROR_NONE)
    {
        LSUtils::respondWithError(request, error);
        return true;
    }

    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    
    return true;
}

bool UwbServiceManager::openSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    pbnjson::JValue responseObj = pbnjson::Object();
    
    pbnjson::JValue requestObj;
	int parseError = 0;
    LSError lsError;
    bool isSubscription = false;
    
    if(mModuleInfo.getDiscoveryStatus() == true) {
        LSUtils::respondWithError(request, UWB_ERROR_DISCOVERY_IN_PROGRESS);
        return true;
    }

    const std::string schema = STRICT_SCHEMA(PROPS_3(PROP(timeOut, integer), PROP(subscribe, boolean), PROP(address, string)) REQUIRED_1(subscribe));

    if(!LSUtils::parsePayload(request.getPayload(),requestObj,schema,&parseError))
	{
		if(parseError != JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		else if (!request.isSubscription())
			LSUtils::respondWithError(request, UWB_ERR_MTHD_NOT_SUBSCRIBED);
		else
			LSUtils::respondWithError(request,UWB_ERR_SCHEMA_VALIDATION_FAIL);

		return true;
	}
    
    if (request.isSubscription())
    {
        if (LSSubscriptionAdd(sh, "openSession", message, &lsError) == false) {
            UWB_LOG_ERROR("Failed to add openSession to subscription");

            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &lsError );
            return true;
        }
        isSubscription = true;
    }
    
    if(mModuleInfo.getDeviceRole() == "controller") {
        if (requestObj.hasKey("address")) {
            std::string macAddress = requestObj["address"].asString();

            UwbErrorCodes error = UWB_ERROR_NONE;
            error = mUwbAdaptor->openSession(macAddress);

            if (error != UWB_ERROR_NONE)
            {
                LSUtils::respondWithError(request, error);
                return true;
            }
        }        
    }
    
    if(mModuleInfo.getDeviceRole() == "controlee") {    
        if (requestObj.hasKey("timeOut"))
        {
            int32_t timeOut = 0;
            timeOut = requestObj["timeOut"].asNumber<int32_t>();
            if (timeOut < 0)
            {
                    LSUtils::respondWithError(request, retrieveErrorText(UWB_ERR_ADV_TIMEOUT_NEG_VALUE) + std::to_string(timeOut), UWB_ERR_ADV_TIMEOUT_NEG_VALUE);
                    return true;
            }
            UwbErrorCodes error = UWB_ERROR_NONE;
            error = mUwbAdaptor->openSessionControlee(timeOut);

            if (error != UWB_ERROR_NONE)
            {
                LSUtils::respondWithError(request, error);
                return true;
            }
        }  
    }
    
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", isSubscription);
    LSUtils::postToClient(request, responseObj);
    
    return true;
}

bool UwbServiceManager::closeSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    pbnjson::JValue responseObj = pbnjson::Object();
    
    if(mModuleInfo.getDeviceRole() == "controlee") {
        LSUtils::respondWithError(request, UWB_ERROR_UNSUPPORTED_API_CONTROLEE);
        return true;
    }
    
    pbnjson::JValue requestObj;
	int parseError = 0;
    
    const std::string schema = STRICT_SCHEMA(PROPS_1(PROP(sessionId, integer)) REQUIRED_1(sessionId));
    
    if(!LSUtils::parsePayload(request.getPayload(),requestObj,schema,&parseError))
	{
		if(parseError != JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		else
			LSUtils::respondWithError(request,UWB_ERR_SCHEMA_VALIDATION_FAIL);

		return true;
	}
    
    if (requestObj.hasKey("sessionId"))
    {
        uint8_t sessionId = 0;
        sessionId = requestObj["sessionId"].asNumber<int32_t>();
        
        UwbErrorCodes error = UWB_ERROR_NONE;
        error = mUwbAdaptor->closeSession(sessionId);

        if (error != UWB_ERROR_NONE)
        {
            LSUtils::respondWithError(request, error);
            return true;
        }
    }    
    
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    return true;
}

bool UwbServiceManager::startRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    pbnjson::JValue responseObj = pbnjson::Object();
    
    if(mModuleInfo.getDeviceRole() == "controlee") {
        LSUtils::respondWithError(request, UWB_ERROR_UNSUPPORTED_API_CONTROLEE);
        return true;
    }
    
    pbnjson::JValue requestObj;
	int parseError = 0;
    
    const std::string schema = STRICT_SCHEMA(PROPS_1(PROP(sessionId, integer)) REQUIRED_1(sessionId));
    
    if(!LSUtils::parsePayload(request.getPayload(),requestObj,schema,&parseError))
	{
		if(parseError != JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		else
			LSUtils::respondWithError(request,UWB_ERR_SCHEMA_VALIDATION_FAIL);

		return true;
	}
    
    if (requestObj.hasKey("sessionId"))
    {
        uint8_t sessionId = 0;
        sessionId = requestObj["sessionId"].asNumber<int32_t>();
        
        UwbErrorCodes error = UWB_ERROR_NONE;
        error = mUwbAdaptor->startRanging(sessionId);

        if (error != UWB_ERROR_NONE)
        {
            LSUtils::respondWithError(request, error);
            return true;
        }
    }    
    
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    return true;
}

bool UwbServiceManager::stopRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
    pbnjson::JValue responseObj = pbnjson::Object();
    
    if(mModuleInfo.getDeviceRole() == "controlee") {
        LSUtils::respondWithError(request, UWB_ERROR_UNSUPPORTED_API_CONTROLEE);
        return true;
    }
    
    pbnjson::JValue requestObj;
	int parseError = 0;
    
    const std::string schema = STRICT_SCHEMA(PROPS_1(PROP(sessionId, integer)) REQUIRED_1(sessionId));
    
    if(!LSUtils::parsePayload(request.getPayload(),requestObj,schema,&parseError))
	{
		if(parseError != JSON_PARSE_SCHEMA_ERROR)
			LSUtils::respondWithError(request, UWB_ERR_BAD_JSON);
		else
			LSUtils::respondWithError(request,UWB_ERR_SCHEMA_VALIDATION_FAIL);

		return true;
	}
    
    if (requestObj.hasKey("sessionId"))
    {
        uint8_t sessionId = 0;
        sessionId = requestObj["sessionId"].asNumber<int32_t>();
        
        UwbErrorCodes error = UWB_ERROR_NONE;
        error = mUwbAdaptor->stopRanging(sessionId);

        if (error != UWB_ERROR_NONE)
        {
            LSUtils::respondWithError(request, error);
            return true;
        }
    }    
    
    responseObj.put("returnValue", true);
    LSUtils::postToClient(request, responseObj);
    return true;
}

void UwbServiceManager::notifyModuleStateChanged(const std::string&  moduleState) {
    if(moduleState == mModuleInfo.getModuleState())
        return;
    
    mModuleInfo.setModuleState(moduleState);
    notifySubscribersModuleStatus();
}

void UwbServiceManager::notifyDeviceNameChanged(const std::string& deviceName) {
    if(deviceName == mModuleInfo.getDeviceName())
        return;
    
    mModuleInfo.setDeviceName(deviceName);
    notifySubscribersModuleStatus();
}

void UwbServiceManager::notifyDiscoveryStatus(bool discoveryStatus) {
    if(discoveryStatus == mModuleInfo.getDiscoveryStatus())
        return;
    
    mModuleInfo.setDiscoveryStatus(discoveryStatus);
    notifySubscribersModuleStatus();
}

void UwbServiceManager::notifyDeviceRoleChanged(const std::string& deviceRole) {
    if(deviceRole == mModuleInfo.getDeviceRole())
        return;
    
    mModuleInfo.setDeviceRole(deviceRole);
    notifySubscribersModuleStatus();
}

void UwbServiceManager::notifyDeviceModeChanged(const std::string& deviceMode) {
    if(deviceMode == mModuleInfo.getDeviceMode())
        return;
    
    mModuleInfo.setDeviceMode(deviceMode);
    notifySubscribersModuleStatus();
}

void UwbServiceManager::notifySubscribersModuleStatus() {
	pbnjson::JValue responseObj = pbnjson::Object();
	appendCurrentStatus(responseObj);
	responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);
    
    LSError lserror;
    LSErrorInit(&lserror);
    if (!LSSubscriptionReply(mServiceHandle, "getStatus", responseObj.stringify().c_str(), &lserror))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lserror);
        return;
    }

    LSErrorFree(&lserror);
    
//	LSUtils::postToSubscriptionPoint(std::atomic_load(&mGetStatusSubscriptions), responseObj);
}

void UwbServiceManager::appendCurrentStatus(pbnjson::JValue &object) {
    object.put("moduleState", mModuleInfo.getModuleState());
    object.put("fwVersion", mModuleInfo.getFwVersion());
    object.put("fwCrc", mModuleInfo.getFwCrc());
    object.put("deviceRole", mModuleInfo.getDeviceRole());
    object.put("deviceMode", mModuleInfo.getDeviceMode());
    object.put("pairingFlag", mModuleInfo.getPairingFlag());
    object.put("discoveryStatus", mModuleInfo.getDiscoveryStatus());
    object.put("uwbMacAddress", mModuleInfo.getUwbMacAddress());
    object.put("deviceName", mModuleInfo.getDeviceName());
}

void UwbServiceManager::notifyScanResult(const std::string& macAddress, const std::string& deviceName) {
    pbnjson::JValue responseObj = pbnjson::Object();
    mRemoteDeviceMap.emplace(std::pair<std::string , std::string>(macAddress, deviceName));
    
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);    
    pbnjson::JValue devicesArray = pbnjson::Array();
    
    for(auto itr: mRemoteDeviceMap) {
        pbnjson::JValue deviceObj = pbnjson::Object();    
        deviceObj.put("address", itr.first);
        deviceObj.put("name", itr.second);    
        devicesArray.append(deviceObj);
    }
    responseObj.put("devices", devicesArray);
    
    LSError lsError;
    LSErrorInit(&lsError);
    if (!LSSubscriptionReply(mServiceHandle, "startDiscovery", responseObj.stringify().c_str(), &lsError))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lsError);
        return;
    }

    LSErrorFree(&lsError);
}

void UwbServiceManager::notifyOpenSessionResponse(uint8_t sessionId) {
    pbnjson::JValue responseObj = pbnjson::Object();
    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);
    responseObj.put("sessionId", sessionId);
    
    LSError lsError;
    LSErrorInit(&lsError);
    if (!LSSubscriptionReply(mServiceHandle, "openSession", responseObj.stringify().c_str(), &lsError))
    {
        UWB_LOG_INFO("subscription reply error!!");
        LSErrorFree(&lsError);
        return;
    }

    LSErrorFree(&lsError);
}