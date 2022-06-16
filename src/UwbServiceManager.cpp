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

    mUwbAdaptor->getUwbServiceState(sh, message);

    return true;
}

bool UwbServiceManager::getUwbSpecificInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->getUwbSpecificInfo(sh, message);
   
    return true;
}

bool UwbServiceManager::getRangingInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->getRangingInfo(sh, message);
    
    return true;
}

bool UwbServiceManager::startDiscovery(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );
    LS::Message request(message);
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
    pbnjson::JValue responseObj = pbnjson::Object();
    
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

    const std::string schema = STRICT_SCHEMA(PROPS_1(PROP(role, string)) REQUIRED_1(role));

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

        error = mUwbAdaptor->setState(role);

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

    mUwbAdaptor->stopDiscovery(message);
    
    return true;
}

bool UwbServiceManager::openSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->openSession(message);
    
    return true;
}

bool UwbServiceManager::closeSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->closeSession(message);
    
    return true;
}

bool UwbServiceManager::startRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->startRanging(message);
    
    return true;
}

bool UwbServiceManager::stopRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor->stopRanging(message);
    
    return true;
}

bool UwbServiceManager::notifyDiscoveryResult() {
    UWB_LOG_INFO("notifyDiscoveryResult API Called %s", __FUNCTION__ );
    return true;
}


void UwbServiceManager::notifyModuleStateChanged(const std::string&  moduleState) {
    if(moduleState == mModuleInfo.getModuleState())
        return;
    
    mModuleInfo.setModuleState(moduleState);
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
    object.put("uwbMacAddress", mModuleInfo.getUwbMacAddress());
}