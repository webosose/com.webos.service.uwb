#ifndef _UWBEVENTLISTENER_H
#define _UWBEVENTLISTENER_H

#include "CallbackInterface.h"
#include "UwbServiceManager.h"
#include <chrono>

class UwbEventListener : public CallbackInterface {
public:    
    //CallbackInterface implementation
    void updateModuleStateChanged(const std::string& moduleState) override;
    void updateDeviceTypeChanged(uint8_t deviceType) override;
    void updateDeviceModeChanged(uint8_t deviceMode) override;
    void updateDeviceNameChanged(const std::string& deviceName) override;
    void updateRangingInfo(uint8_t condition, uint8_t sessionId, int64_t angle, int64_t distance) override;
    void updateScanResult(const std::string& macAddress, const std::string& deviceName) override;
    void updateModuleStatus() override;
    void updateDiscoveryStatus(bool discoveryStatus) override;
    void updateOpenSessionResponse(uint8_t sessionId) override;
private:

};
#endif