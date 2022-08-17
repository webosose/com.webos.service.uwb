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
	virtual void updatePairingInfo(const pbnjson::JValue& pairingArray, uint8_t pairingCount) = 0; 
    virtual void updateModuleStatus() = 0;
    virtual void updateDiscoveryStatus(bool discoveryStatus) = 0;
    virtual void updateOpenSessionResponse(uint8_t sessionId) = 0;
};

#endif