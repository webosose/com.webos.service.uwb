#include "UwbRangingInfo.h"

UwbRangingInfo::UwbRangingInfo() : m_sessionId(0),
                                   m_condition(0),
                                   m_connectionStatus(true) {
    m_receivedData = new RangingData();
    mDistanceMeasure.reset();
    mAngleMeasure.reset();
}

UwbRangingInfo::~UwbRangingInfo() {
    if(m_receivedData) {
            delete m_receivedData;
    }
}
