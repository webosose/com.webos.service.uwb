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
#include "UwbServiceManager.h"
#include "CallbackInterface.h"
#include "UartConstants.h"

using namespace std;

namespace LS
{
    class Message;
}

class UwbAdaptor : public std::enable_shared_from_this<UwbAdaptor>, public UwbAdapterInterface, public CallbackInterface {
public:
    UwbAdaptor();
    ~UwbAdaptor();
    UwbAdaptor(const UwbAdaptor&) = delete;
    UwbAdaptor& operator=(const UwbAdaptor&) = delete;    
 
    bool init(LSHandle *sh);
    void setDeviceInterface(std::shared_ptr<UartSerial> uartSerial);
    bool getUwbServiceState(LSHandle *sh, LSMessage *message);
    bool getUwbSpecificInfo(LSHandle *sh, LSMessage *message);
    bool getRangingInfo(LSHandle *sh, LSMessage *message);
    UwbErrorCodes setUwbModuleState(const std::string& moduleState);
    UwbErrorCodes getStatus();
    bool getPairedSessions(LSMessage *message);
    UwbErrorCodes setState(const std::string& deviceType);
    UwbErrorCodes startDiscovery(int32_t discoveryTimeout);
    bool stopDiscovery(LSMessage *message);
    bool openSession(LSMessage *message);
    bool closeSession(LSMessage *message);
    bool startRanging(LSMessage *message);
    bool stopRanging(LSMessage *message);
    
    void setServiceState(bool serviceState) {
        m_isServiceAvailable = serviceState;
    }

    void updateServiceState(bool isServiceAvailable); //Not supported currently
    void updateSpecificInfo(bool modState, string fwVersion, string fwCrc);
    void updateRangingInfo(int condition, string remoteDevAddr, int64_t angle, int64_t distance);
    void updateDisconnectedDevice(uint16_t deviceID);
    void updateModuleStateChanged(bool moduleState);

    void notifySubscriberServiceState(bool isServiceAvailable);
    void notifySubscriberSpecificInfo(UwbSpecInfo& info);
    void notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo>& rangingInfo);

private:    
    LSHandle *mLSHandle = nullptr;
    static UwbSpecInfo *mUwbSpecInfo;
    bool m_isServiceAvailable{false};
    bool m_connectionStatus{false};
    int64_t m_sessionId{0};
    std::unique_ptr<UwbRangingInfo> mSavedUwbRangingInfo; // for saving up-to-date rangingInfo
    std::unique_ptr<IResponseBuilder> mResponseBuilder;
    std::shared_ptr<UartSerial> mUartSerial = nullptr;
};

#endif//H_UwbAdaptor
