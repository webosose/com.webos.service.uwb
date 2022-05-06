#include "UwbServiceManager.h"
#include "LunaUwbServiceUtil.h"
#include <pbnjson.hpp>

template <class T>
LSMethod UwbServiceManager<T>::serviceMethods[] = {
    { "getUwbServiceState", UwbServiceManager<T>::_getUwbServiceState },
    { "getUwbSpecificInfo", UwbServiceManager<T>::_getUwbSpecificInfo },
    { "getRangingInfo",     UwbServiceManager<T>::_getRangingInfo     },
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
