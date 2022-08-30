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

#ifndef _UWBADAPTERINTERFACE_H
#define _UWBADAPTERINTERFACE_H

#include <lunaservice.h>
#include <string>
#include "UwbErrors.h"
#include "UwbSpecInfo.h"
#include "UwbRangingInfo.h"

class UartSerial;
class UwbAdapterInterface {
public:
    virtual ~UwbAdapterInterface() = default;

    virtual bool init(LSHandle *sh) = 0;
    virtual void setDeviceInterface(std::shared_ptr<UartSerial> uartSerial) = 0;
    virtual UwbErrorCodes setUwbModuleState(const std::string& moduleState) = 0;
    virtual UwbErrorCodes setDeviceType(const std::string& deviceType) = 0;
    virtual UwbErrorCodes setDeviceName(const std::string& deviceName) = 0;
    virtual UwbErrorCodes setDeviceMode(const std::string& deviceMode) = 0;
    virtual UwbErrorCodes startDiscovery(int32_t discoveryTimeout) = 0;
    virtual UwbErrorCodes openSession(const std::string& address) = 0;
    virtual UwbErrorCodes openSessionControlee(int32_t advTimeout) = 0;
    virtual UwbErrorCodes stopDiscovery() = 0;
    virtual UwbErrorCodes closeSession(uint8_t sessionId) = 0;
    virtual UwbErrorCodes startRanging(uint8_t sessionId) = 0;
    virtual UwbErrorCodes stopRanging(uint8_t sessionId) = 0;
};

#endif
