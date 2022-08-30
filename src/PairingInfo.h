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

#ifndef _PAIRINGINFO_H
#define _PAIRINGINFO_H

#include <string>
#include <pbnjson.hpp>

class PairingInfo {
public:

   static PairingInfo& getInstance() {
        static PairingInfo instance;
        return instance;
   }

   uint8_t getPairingCount();
   pbnjson::JValue getSessionInfo();
   void setPairingCount(uint8_t pairingCount);
   void setSessionInfo(pbnjson::JValue& pairingArray);

private:
    PairingInfo();
    ~PairingInfo()= default;
    PairingInfo(const PairingInfo&)= delete;
    PairingInfo& operator=(const PairingInfo&)= delete;

    uint8_t mPairingCount=0;
    pbnjson::JValue mSessionArray = pbnjson::Array();

};

#endif

