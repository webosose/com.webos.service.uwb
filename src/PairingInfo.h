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

