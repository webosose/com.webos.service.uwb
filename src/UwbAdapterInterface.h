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
    virtual bool getUwbServiceState(LSHandle *sh, LSMessage *message) = 0;
    virtual bool getUwbSpecificInfo(LSHandle *sh, LSMessage *message) = 0;
    virtual bool getRangingInfo(LSHandle *sh, LSMessage *message) = 0;
    virtual UwbErrorCodes setUwbModuleState(const std::string& moduleState) = 0;
    virtual UwbErrorCodes getStatus() = 0;
    virtual bool getPairedSessions(LSMessage *message) = 0;
    virtual UwbErrorCodes setDeviceType(const std::string& deviceType) = 0;
    virtual UwbErrorCodes setDeviceName(const std::string& deviceName) = 0;
    virtual UwbErrorCodes setDeviceMode(const std::string& deviceMode) = 0;
    virtual UwbErrorCodes startDiscovery(int32_t discoveryTimeout) = 0;
    virtual bool stopDiscovery(LSMessage *message) = 0;
    virtual bool openSession(LSMessage *message) = 0;
    virtual bool closeSession(LSMessage *message) = 0;
    virtual bool startRanging(LSMessage *message) = 0;
    virtual bool stopRanging(LSMessage *message) = 0;
};

#endif
