#include "UwbServiceManager.h"
#include "LunaUwbServiceUtil.h"
#include <pbnjson.hpp>

LSMethod UwbServiceManager::serviceMethods[] = {
    { "getUwbServiceState", getUwbServiceState },
//    { "getUwbSpecificInfo", getUwbSpecificInfo },
//    { "getRangingInfo",     getRangingInfo     },
    { 0               ,      0                 }
};

UwbServiceManager *UwbServiceManager::getInstance() {
    static UwbServiceManager _instance;
    return &_instance;
}

UwbServiceManager::UwbServiceManager():
        mMainLoop(nullptr),
        mUwbAdaptor(nullptr),
        mUwbSessionCtl(nullptr) {

}

UwbServiceManager::~UwbServiceManager(){

}

bool UwbServiceManager::init(GMainLoop *mainLoop) {

    if (UwbServiceRegister(UWB_SERVICE_NAME, mainLoop, &mServiceHandle) == false) {
        UWB_LOG_ERROR("com.webos.service.uwb service registration failed");
        return false;
    }

    UWB_LOG_DEBUG("mServiceHandle =%p", mServiceHandle);

//    mUwbAdaptor = UwbAdaptor::getInstance();
//    if(!mUwbAdaptor) {
//        mUwbAdaptor->init();
//    } else {
//        UWB_LOG_DEBUG("mUwbAdaptor not created");
//        return false;
//    }
//
//    mUwbSessionCtl = UwbSessionControl::getInstance();
//    if(!mUwbSessionCtl) {
//        mUwbSessionCtl->init();
//    } else {
//        UWB_LOG_DEBUG("mUwbSessionCtl not created");
//        return false;
//    }

    return true;
}

bool UwbServiceManager::UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle) {

    //bool bRetVal; // changed to local variable from class variable
    LSError mLSError;
    LSErrorInit(&mLSError);

    //service registration
    if(!LSRegister(srvcname, mServiceHandle, &mLSError))
    {
        UWB_LOG_ERROR("LS_REGISTER :Unable to register to luna-bus");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }

    //Gmain attach
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

    LSError mLSError;
    bool isSubscription = false;
    pbnjson::JValue responseObj = pbnjson::Object();

    LSErrorInit(&mLSError);

    //schema check routine

    //subscription check
    if (LSMessageIsSubscription(message)) {
        isSubscription = true;
        if (LSSubscriptionAdd(sh, "getUwbServiceState", message, &mLSError) == false) {
            UWB_LOG_ERROR("Failed to add getUwbServiceState to subscription");
            //send error response
            responseObj.put("returnValue", false);
            responseObj.put("errorCode", UWB_UNKNOWN_ERROR);
            responseObj.put("errorText", "Unknwon");
            LSMessageReply(sh,message, responseObj.stringify().c_str() , &mLSError ); // send error message
            return true;
        }
    }

    // serialization and send

    if (responseObj.isNull())
            return false;

    responseObj.put("returnValue", true);
    responseObj.put("serviceAvailability", true);
    responseObj.put("subscribed", isSubscription);

    if (!LSMessageReply(sh, message, responseObj.stringify().c_str(), &mLSError))
    {
        UWB_LOG_ERROR("HANDLE_getUwbServiceState Message reply error!!");
        LSErrorPrint(&mLSError, stdout);

        return false;
    }
    return true;
}
