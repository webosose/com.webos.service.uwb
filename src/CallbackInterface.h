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

#ifndef _CALLBACKINTERFACE_H
#define _CALLBACKINTERFACE_H

#include <string>
#include <pbnjson.hpp>

class CallbackInterface {
public:
    virtual ~CallbackInterface() = default;
    virtual void updateModuleStateChanged(const std::string& moduleState) = 0;
    virtual void updateDeviceTypeChanged(uint8_t deviceType) = 0;
    virtual void updateDeviceModeChanged(uint8_t deviceMode) = 0;
    virtual void updateDeviceNameChanged(const std::string& deviceName) = 0;
    virtual void updateRangingInfo(uint8_t condition, uint8_t sessionId, int64_t angle, int64_t distance) = 0;
    virtual void updateScanResult(const std::string& macAddress, const std::string& deviceName) = 0;
    virtual void updatePairingInfo() = 0;
    virtual void updateModuleStatus() = 0;
    virtual void updateDiscoveryStatus(bool discoveryStatus) = 0;
    virtual void updatePairingFlag(bool pairingFlag) = 0;
    virtual void updateOpenSessionResponse(uint8_t sessionId) = 0;
};

#endif