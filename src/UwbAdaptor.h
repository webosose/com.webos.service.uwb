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

#ifndef H_UwbAdaptor
#define H_UwbAdaptor

#include <pbnjson.hpp>
#include <lunaservice.h>
#include <luna-service2/lunaservice.hpp>
#include <pthread.h>
#include <string>
#include <memory>
#include "UwbSpecInfo.h"
#include "UwbRangingInfo.h"
#include "UwbResponseBuilder.h"
#include "UwbErrors.h"
#include "UartSerial.h"
#include "UwbAdapterInterface.h"
#include "CallbackInterface.h"
#include "UartConstants.h"

using namespace std;

namespace LS
{
    class Message;
}

class UwbAdaptor : public UwbAdapterInterface {
public:
    UwbAdaptor();
    ~UwbAdaptor();
    UwbAdaptor(const UwbAdaptor&) = delete;
    UwbAdaptor& operator=(const UwbAdaptor&) = delete;

    bool init(LSHandle *sh);
    void setDeviceInterface(std::shared_ptr<UartSerial> uartSerial);

    UwbErrorCodes setUwbModuleState(const std::string& moduleState) override;
    UwbErrorCodes setDeviceType(const std::string& deviceType) override;
    UwbErrorCodes setDeviceName(const std::string& deviceName) override;
    UwbErrorCodes setDeviceMode(const std::string& deviceMode) override;
    UwbErrorCodes startDiscovery(int32_t discoveryTimeout) override;
    UwbErrorCodes openSession(const std::string& address) override;
    UwbErrorCodes openSessionControlee(int32_t advTimeout) override;
    UwbErrorCodes stopDiscovery() override;
    UwbErrorCodes closeSession(uint8_t sessionId) override;
    UwbErrorCodes startRanging(uint8_t sessionId) override;
    UwbErrorCodes stopRanging(uint8_t sessionId) override;

private:
    LSHandle *mLSHandle = nullptr;
    std::shared_ptr<UartSerial> mUartSerial = nullptr;
};

#endif
