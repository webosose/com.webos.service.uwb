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

