#ifndef _CALLBACKINTERFACE_H
#define _CALLBACKINTERFACE_H

#include <string>

class CallbackInterface {
public:
    virtual ~CallbackInterface() = default;
    virtual void updateServiceState(bool isServiceAvailable) = 0; //Not supported currently
    virtual void updateSpecificInfo(bool modState, std::string fwVersion, std::string fwCrc) = 0;
    virtual void updateRangingInfo(int condition, std::string remoteDevAddr, int64_t angle, int64_t distance) = 0;
    virtual void updateDisconnectedDevice(uint16_t deviceID) = 0;
    virtual void updateModuleStateChanged(const std::string& moduleState) = 0;
    virtual void updateDeviceTypeChanged(uint8_t deviceType) = 0;
    virtual void updateDeviceModeChanged(uint8_t deviceMode) = 0;
    virtual void updateDeviceNameChanged(const std::string& deviceName) = 0;
};

#endif