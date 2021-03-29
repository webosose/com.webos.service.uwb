#ifndef H_UwbServiceManager
#define H_UwbServiceManager

//include
#include "UwbServiceManager.h"
#include "UwbLogging.h"
#include "UwbAdaptor.h"
#include "UwbSessionControl.h"
#include <lunaservice.h>
#include <glib.h>
#include <string>

//definition
#define UWB_SERVICE_NAME           "com.webos.service.uwb"


class UwbServiceManager
{
public:
    virtual ~UwbServiceManager();
    bool init(GMainLoop *);
    bool UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle);
    bool deinit();
    static UwbServiceManager *getInstance();
    static bool getUwbServiceState(LSHandle *sh, LSMessage *message, void *data);
    static bool getUwbSpecificInfo(LSHandle *sh, LSMessage *message, void *data);
    static bool getRangingInfo(LSHandle *sh, LSMessage *message, void *data);

    static UwbServiceManager *uwbService;
    static LSMethod serviceMethods[];

private:
    LSHandle *mServiceHandle;
    GMainLoop *mMainLoop;
    UwbAdaptor *mUwbAdaptor;
    UwbSessionControl *mUwbSessionCtl;
    UwbServiceManager();

    bool mmodState;
    std::string mfwVersion;
    std::string mfwCrc;


};


#endif
