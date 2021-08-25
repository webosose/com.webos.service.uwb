#ifndef H_UwbAdaptor
#define H_UwbAdaptor

#include <pbnjson.hpp>
#include <lunaservice.h>
#include <luna-service2/lunaservice.hpp>
#include <pthread.h>
#include <string>
#include "UwbSpecInfo.h"
#include "UwbRangingInfo.h"

using namespace std;

namespace LS
{
    class Message;
}

class UwbAdaptor {

public:

    static UwbAdaptor *getInstance();

    ~UwbAdaptor();
    bool init(LSHandle *sh);

    bool getUwbServiceState(LSHandle *sh, LSMessage *message);
    bool getUwbSpecificInfo(LSHandle *sh, LSMessage *message);
    bool getRangingInfo(LSHandle *sh, LSMessage *message);

    void updateServiceState(bool isServiceAvailable); //Not supported currently
    void updateSpecificInfo(bool modState, string fwVersion, string fwCrc);
    void updateRangingInfo(int condition, string remoteDevAddr, int64_t angle, int64_t distance);
    void updateDisconnectedDevice(uint16_t deviceID);

    void notifySubscriberServiceState(bool isServiceAvailable);
    void notifySubscriberSpecificInfo(UwbSpecInfo& info);
    void notifySubscriberRangingInfo(UwbRangingInfo& rangingInfo);

    void writeServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable);
    void writeSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info);
    void writeRangingInfo(pbnjson::JValue &responseObj, UwbRangingInfo& rangingInfo);

    void setServiceState(bool serviceState) {
        m_isServiceAvailable = serviceState;
    }

private:
    UwbAdaptor();
    LSHandle *mLSHandle = nullptr;
    static UwbAdaptor *mUwbAdaptor;
    static UwbSpecInfo *mUwbSpecInfo;
    bool m_isServiceAvailable{false};
    bool m_connectionStatus{false};
    int64_t m_sessionId{0};
    UwbRangingInfo *mSavedUwbRangingInfo = nullptr; // for saving up-to-date rangingInfo
    UwbRangingInfo *mRangingInfo = nullptr;
    UwbRangingInfo *disConnRangingInfo = nullptr;

};

#endif//H_UwbAdaptor
