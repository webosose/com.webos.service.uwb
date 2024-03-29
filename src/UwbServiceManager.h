// @@@LICENSE
//
//      Copyright (c) 2022 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef H_UwbServiceManager
#define H_UwbServiceManager

#include "UwbLogging.h"
#include "UwbAdapterInterface.h"
#include "UwbSessionControl.h"
#include "ModuleInfo.h"
#include "UwbResponseBuilder.h"
#include "PairingInfo.h"
#include "ls2utils.h"
#include <lunaservice.h>
#include <luna-service2/lunaservice.hpp>
#include <glib.h>
#include <string>
#include <memory>

#define UWB_SERVICE_NAME           "com.webos.service.uwb"

#define UWB_SERVICE_METHOD(name) \
    bool name(LSHandle *sh, LSMessage *message, void *data); \
    static bool _##name(LSHandle *sh, LSMessage *message, void *data) { \
        return ((UwbServiceManager*)data)->name(sh, message, NULL); \
    }

class UwbServiceManager {
public:
    virtual ~UwbServiceManager();
    bool init(GMainLoop *, std::shared_ptr<UwbAdapterInterface> adapter);
    bool UwbServiceRegister(const char *srvcname, GMainLoop *mainLoop, LSHandle **mServiceHandle);
    bool deinit();
    static UwbServiceManager *getInstance();

    static LSMethod serviceMethods[];
    void notifyModuleStateChanged(const std::string&  moduleState);
    void notifyDeviceNameChanged(const std::string& deviceName);
    void notifyDeviceRoleChanged(const std::string& deviceRole);
    void notifyDeviceModeChanged(const std::string& deviceMode);
    void notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo> rangingInfo, uint8_t sessionId);
    void notifyScanResult(const std::string& macAddress, const std::string& deviceName);
    void notifyPairingInfo();
    void notifySubscribersModuleStatus();
    void notifyDiscoveryStatus(bool discoveryStatus);
    void notifyPairingFlagChanged(bool pairingFlag);
    void notifyOpenSessionResponse(uint8_t sessionId);
    void writeServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable);

private:
    UwbServiceManager();
    void appendCurrentStatus(pbnjson::JValue &object);

    UWB_SERVICE_METHOD(getUwbServiceState);
    UWB_SERVICE_METHOD(getUwbSpecificInfo);
    UWB_SERVICE_METHOD(getRangingInfo);
    UWB_SERVICE_METHOD(getStatus);
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
    inline static std::shared_ptr<UwbAdapterInterface> mUwbAdaptor{}; //couldn't make it non-static because it's used in a static function
    UwbSessionControl *mUwbSessionCtl;
    inline static ModuleInfo& mModuleInfo = ModuleInfo::getInstance();
    inline static PairingInfo& mPairingInfo = PairingInfo::getInstance();
    inline static std::unique_ptr<UwbRangingInfo> mSavedUwbRangingInfo = nullptr; // for saving latest rangingInfo
    inline static std::unique_ptr<IResponseBuilder> mResponseBuilder{std::make_unique<UwbResponseBuilder>()};
    std::unordered_map<std::string , std::string> mRemoteDeviceMap{};
    inline static UwbSpecInfo& mUwbSpecInfo = UwbSpecInfo::getInstance();
};


#endif
