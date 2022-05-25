#include "UwbServiceManager.h"
#include "LunaUwbServiceUtil.h"
#include <pbnjson.hpp>

template <class T>
LSMethod UwbServiceManager<T>::serviceMethods[] = {
    { "getUwbServiceState", UwbServiceManager<T>::_getUwbServiceState },
    { "getUwbSpecificInfo", UwbServiceManager<T>::_getUwbSpecificInfo },
    { "getRangingInfo",     UwbServiceManager<T>::_getRangingInfo },
    { "setUwbModuleState", UwbServiceManager<T>::_setUwbModuleState },
    { "getUwbStatus", UwbServiceManager<T>::_getUwbStatus },
    { "getPairedSessions",     UwbServiceManager<T>::_getPairedSessions },
    { "setState",     UwbServiceManager<T>::_setState },
    { "startDiscovery",     UwbServiceManager<T>::_startDiscovery },
    { "stopDiscovery", UwbServiceManager<T>::_stopDiscovery },
    { "openSession", UwbServiceManager<T>::_openSession },
    { "closeSession",     UwbServiceManager<T>::_closeSession },
    { "startRanging", UwbServiceManager<T>::_startRanging },
    { "stopRanging", UwbServiceManager<T>::_stopRanging },  
    { 0               ,      0                 }
};

template <class T>
UwbServiceManager<T> *UwbServiceManager<T>::getInstance() {
    if(mUwbServiceMgr == nullptr) mUwbServiceMgr = new UwbServiceManager<T>();
    return mUwbServiceMgr;
}

template <class T>
UwbServiceManager<T>::UwbServiceManager():
        mMainLoop(nullptr),
        mUwbSessionCtl(nullptr) {

}

template <class T>
UwbServiceManager<T>::~UwbServiceManager(){
    delete mUwbSessionCtl;
}

template <class T>
bool UwbServiceManager<T>::init(GMainLoop *mainLoop) {

    if (UwbServiceRegister(UWB_SERVICE_NAME, mainLoop, &mServiceHandle) == false) {
        UWB_LOG_ERROR("com.webos.service.uwb service registration failed");
        return false;
    }

    UWB_LOG_DEBUG("mServiceHandle =%p", mServiceHandle);

    UWB_LOG_INFO("uwbServiceManager this = %p", this );
    mUwbAdaptor.init(mServiceHandle);

    return true;
}

template <class T>
bool UwbServiceManager<T>::UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle) {

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

template <class T>
bool UwbServiceManager<T>::deinit() {
    return true;
}

template <class T>
bool UwbServiceManager<T>::getUwbServiceState(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.getUwbServiceState(sh, message);

    return true;
}

template <class T>
bool UwbServiceManager<T>::getUwbSpecificInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.getUwbSpecificInfo(sh, message);
   
    return true;
}

template <class T>
bool UwbServiceManager<T>::getRangingInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.getRangingInfo(sh, message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::startDiscovery(LSHandle *sh, LSMessage *message, void *data) {
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

    error = mUwbAdaptor.startDiscovery(discoveryTimeout);

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

template <class T>
bool UwbServiceManager<T>::setUwbModuleState(LSHandle *sh, LSMessage *message, void *data) {
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

        error = mUwbAdaptor.setUwbModuleState(moduleState);

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

template <class T>
bool UwbServiceManager<T>::getUwbStatus(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.getUwbStatus(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::getPairedSessions(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.getPairedSessions(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::setState(LSHandle *sh, LSMessage *message, void *data) {
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

        error = mUwbAdaptor.setState(role);

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

template <class T>
bool UwbServiceManager<T>::stopDiscovery(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.stopDiscovery(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::openSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.openSession(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::closeSession(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.closeSession(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::startRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.startRanging(message);
    
    return true;
}

template <class T>
bool UwbServiceManager<T>::stopRanging(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    mUwbAdaptor.stopRanging(message);
    
    return true;
}
