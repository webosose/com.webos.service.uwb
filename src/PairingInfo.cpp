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

#include "PairingInfo.h"

PairingInfo::PairingInfo() {

}

uint8_t PairingInfo::getPairingCount(){
    return mPairingCount;
}

pbnjson::JValue PairingInfo::getSessionInfo(){
    return mSessionArray;
}

void PairingInfo::setPairingCount(uint8_t pairingCount){
    mPairingCount = pairingCount;
}

void PairingInfo::setSessionInfo(pbnjson::JValue& pairingArray){
  mSessionArray = pairingArray;
}

