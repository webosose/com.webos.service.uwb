#include "UwbRangingInfo.h"

UwbRangingInfo::UwbRangingInfo() : m_sessionId(0) {
    m_receivedData = new RangingData();
}

UwbRangingInfo::~UwbRangingInfo() {

}
