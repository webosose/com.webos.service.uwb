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

    UwbErrorCodes getStatus();
    UwbErrorCodes setUwbModuleState(const std::string& moduleState);
    UwbErrorCodes setDeviceType(const std::string& deviceType);
    UwbErrorCodes setDeviceName(const std::string& deviceName);
    UwbErrorCodes setDeviceMode(const std::string& deviceMode);
    UwbErrorCodes startDiscovery(int32_t discoveryTimeout);
    bool getPairedSessions(LSMessage *message);
    bool stopDiscovery(LSMessage *message);
    bool openSession(LSMessage *message);
    bool closeSession(LSMessage *message);
    bool startRanging(LSMessage *message);
    bool stopRanging(LSMessage *message);

private:    
    LSHandle *mLSHandle = nullptr;
    std::shared_ptr<UartSerial> mUartSerial = nullptr;
};

#endif//H_UwbAdaptor
