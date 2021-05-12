#include "UwbServiceManager.h"
#include "LunaUwbServiceUtil.h"
#include <pbnjson.hpp>

LSMethod UwbServiceManager::serviceMethods[] = {
    { "getUwbServiceState", UwbServiceManager::_getUwbServiceState },
    { "getUwbSpecificInfo", UwbServiceManager::_getUwbSpecificInfo },
    { "getRangingInfo",     UwbServiceManager::_getRangingInfo     },
    { 0               ,      0                 }
};

UwbAdaptor* UwbServiceManager::mUwbAdaptor = nullptr;
UwbServiceManager* UwbServiceManager::mUwbServiceMgr = nullptr;

UwbServiceManager *UwbServiceManager::getInstance() {
    if(mUwbServiceMgr == nullptr) mUwbServiceMgr = new UwbServiceManager();
    return mUwbServiceMgr;
}

UwbServiceManager::UwbServiceManager():
        mMainLoop(nullptr),
        mUwbSessionCtl(nullptr) {

}

UwbServiceManager::~UwbServiceManager(){
    delete mUwbAdaptor;
    delete mUwbSessionCtl;
}

bool UwbServiceManager::init(GMainLoop *mainLoop) {

    if (UwbServiceRegister(UWB_SERVICE_NAME, mainLoop, &mServiceHandle) == false) {
        UWB_LOG_ERROR("com.webos.service.uwb service registration failed");
        return false;
    }

    UWB_LOG_DEBUG("mServiceHandle =%p", mServiceHandle);

    mUwbAdaptor = UwbAdaptor::getInstance();

    UWB_LOG_INFO("uwbServiceManager this = %p", this );
    mUwbAdaptor->init(mServiceHandle);
    UWB_LOG_INFO("mUwbAdaptor2 = %p", mUwbAdaptor );

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

    if(mUwbAdaptor != nullptr) {
        mUwbAdaptor->getUwbServiceState(sh, message);
    } else {
        UWB_LOG_INFO("getUwbServiceState, mUwbAdaptor null");
    }
    return true;
}

bool UwbServiceManager::getUwbSpecificInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    if(mUwbAdaptor != nullptr) {
        mUwbAdaptor->getUwbSpecificInfo(sh, message);
    } else {
        UWB_LOG_INFO("getUwbSpecificInfo, mUwbAdaptor null");
    }
    return true;
}

bool UwbServiceManager::getRangingInfo(LSHandle *sh, LSMessage *message, void *data) {
    UWB_LOG_INFO("Luna API Called %s", __FUNCTION__ );

    if(mUwbAdaptor != nullptr) {
        UWB_LOG_INFO("getRangingInfo, mUwbAdaptor not null");
        mUwbAdaptor->getRangingInfo(sh, message);
    } else {
        UWB_LOG_INFO("getRangingInfo, mUwbAdaptor null");
        return false;
    }
    return true;
}
