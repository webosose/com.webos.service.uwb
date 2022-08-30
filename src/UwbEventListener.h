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

#ifndef _UWBEVENTLISTENER_H
#define _UWBEVENTLISTENER_H

#include "CallbackInterface.h"
#include "UwbServiceManager.h"
#include <pbnjson.hpp>
#include <chrono>

class UwbEventListener : public CallbackInterface {
public:
    void updateModuleStateChanged(const std::string& moduleState) override;
    void updateDeviceTypeChanged(uint8_t deviceType) override;
    void updateDeviceModeChanged(uint8_t deviceMode) override;
    void updateDeviceNameChanged(const std::string& deviceName) override;
    void updateRangingInfo(uint8_t condition, uint8_t sessionId, int64_t angle, int64_t distance) override;
    void updateScanResult(const std::string& macAddress, const std::string& deviceName) override;
    void updatePairingInfo();
    void updateModuleStatus() override;
    void updateDiscoveryStatus(bool discoveryStatus) override;
    void updatePairingFlag(bool pairingFlag) override;
    void updateOpenSessionResponse(uint8_t sessionId) override;
private:

};
#endif
