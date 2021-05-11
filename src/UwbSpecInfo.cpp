#include "UwbSpecInfo.h"

UwbSpecInfo* UwbSpecInfo::m_uwbSpecInfo = nullptr;

UwbSpecInfo* UwbSpecInfo::getInstance() {
    if(!m_uwbSpecInfo) m_uwbSpecInfo = new UwbSpecInfo();
    return m_uwbSpecInfo;
}

UwbSpecInfo::UwbSpecInfo() : m_modState(false),
    m_fwVersion("0"),
    m_fwCrc("0"){

}

UwbSpecInfo::~UwbSpecInfo() {

}
