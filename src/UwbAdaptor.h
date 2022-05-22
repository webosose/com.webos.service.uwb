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

using namespace std;

namespace LS
{
    class Message;
}

class UwbAdaptor {

public:

    static UwbAdaptor& getInstance() {
        static UwbAdaptor instance; 
        return instance;
    }    
 
    bool init(LSHandle *sh);

    bool getUwbServiceState(LSHandle *sh, LSMessage *message);
    bool getUwbSpecificInfo(LSHandle *sh, LSMessage *message);
    bool getRangingInfo(LSHandle *sh, LSMessage *message);
    UwbErrorCodes setUwbModuleState(const std::string& moduleState);
    bool getUwbStatus(LSMessage *message);
    bool getPairedSessions(LSMessage *message);
    bool setState(LSMessage *message);
    bool startDiscovery(LSMessage *message);
    bool stopDiscovery(LSMessage *message);
    bool openSession(LSMessage *message);
    bool closeSession(LSMessage *message);
    bool startRanging(LSMessage *message);
    bool stopRanging(LSMessage *message);

    void updateServiceState(bool isServiceAvailable); //Not supported currently
    void updateSpecificInfo(bool modState, string fwVersion, string fwCrc);
    void updateRangingInfo(int condition, string remoteDevAddr, int64_t angle, int64_t distance);
    void updateDisconnectedDevice(uint16_t deviceID);

    void notifySubscriberServiceState(bool isServiceAvailable);
    void notifySubscriberSpecificInfo(UwbSpecInfo& info);
    void notifySubscriberRangingInfo(std::unique_ptr<UwbRangingInfo>& rangingInfo);

    void setServiceState(bool serviceState) {
        m_isServiceAvailable = serviceState;
    }

private:
    UwbAdaptor();
    ~UwbAdaptor()= default;
    UwbAdaptor(const UwbAdaptor&)= delete;
    UwbAdaptor& operator=(const UwbAdaptor&)= delete;
    
    LSHandle *mLSHandle = nullptr;
    static UwbSpecInfo *mUwbSpecInfo;
    bool m_isServiceAvailable{false};
    bool m_connectionStatus{false};
    int64_t m_sessionId{0};
    std::unique_ptr<UwbRangingInfo> mSavedUwbRangingInfo; // for saving up-to-date rangingInfo
    std::unique_ptr<IResponseBuilder> mResponseBuilder;

};

#endif//H_UwbAdaptor
