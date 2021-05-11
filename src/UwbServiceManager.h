#ifndef H_UwbServiceManager
#define H_UwbServiceManager

//include
#include "UwbServiceManager.h"
#include "UwbLogging.h"
#include "UwbAdaptor.h"
#include "UwbSessionControl.h"
#include <lunaservice.h>
#include <luna-service2/lunaservice.hpp>
#include <glib.h>
#include <string>

//definition
#define UWB_SERVICE_NAME           "com.webos.service.uwb"

#define UWB_SERVICE_METHOD(name) \
    bool name(LSHandle *sh, LSMessage *message, void *data); \
    static bool _##name(LSHandle *sh, LSMessage *message, void *data) { \
        return ((UwbServiceManager*)data)->name(sh, message, NULL); \
    }

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

    LSHandle *mServiceHandle;
    GMainLoop *mMainLoop;
    static UwbServiceManager *mUwbServiceMgr;
    static UwbAdaptor *mUwbAdaptor;
    UwbSessionControl *mUwbSessionCtl;

};


#endif
