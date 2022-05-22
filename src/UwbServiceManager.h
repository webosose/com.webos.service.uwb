#ifndef H_UwbServiceManager
#define H_UwbServiceManager

#include "UwbLogging.h"
#include "UwbAdaptor.h"
#include "UwbSessionControl.h"
#include "ls2utils.h"
#include <lunaservice.h>
#include <luna-service2/lunaservice.hpp>
#include <glib.h>
#include <string>

#define UWB_SERVICE_NAME           "com.webos.service.uwb"

#define UWB_SERVICE_METHOD(name) \
    bool name(LSHandle *sh, LSMessage *message, void *data); \
    static bool _##name(LSHandle *sh, LSMessage *message, void *data) { \
        return ((UwbServiceManager*)data)->name(sh, message, NULL); \
    }

template <class T>
class UwbServiceManager
{
public:
    virtual ~UwbServiceManager();
    bool init(GMainLoop *);
    bool UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle);
    bool deinit();
    static UwbServiceManager *getInstance();

    static LSMethod serviceMethods[];

private:
    UwbServiceManager();

    UWB_SERVICE_METHOD(getUwbServiceState);
    UWB_SERVICE_METHOD(getUwbSpecificInfo);
    UWB_SERVICE_METHOD(getRangingInfo);    
    UWB_SERVICE_METHOD(setUwbModuleState);
    UWB_SERVICE_METHOD(getUwbStatus);
    UWB_SERVICE_METHOD(getPairedSessions);
    UWB_SERVICE_METHOD(setState);
    UWB_SERVICE_METHOD(startDiscovery);
    UWB_SERVICE_METHOD(stopDiscovery);
    UWB_SERVICE_METHOD(openSession);
    UWB_SERVICE_METHOD(closeSession);
    UWB_SERVICE_METHOD(startRanging);
    UWB_SERVICE_METHOD(stopRanging);

    LSHandle *mServiceHandle = nullptr;
    GMainLoop *mMainLoop;
    inline static UwbServiceManager *mUwbServiceMgr{nullptr};
    inline static T& mUwbAdaptor = T::getInstance();
    UwbSessionControl *mUwbSessionCtl;
};


#endif
