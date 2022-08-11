#ifndef _IRESPONSEBUILDER_H
#define _IRESPONSEBUILDER_H
#include <memory>
#include <pbnjson.hpp>
#include "UwbRangingInfo.h"
#include "UwbSpecInfo.h"

class IResponseBuilder {
public:
    virtual ~IResponseBuilder() = default;
    virtual void buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo) = 0;  
 //TODO:need to be removed..
 //   virtual void buildSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info) = 0;
 //   virtual void buildServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable) = 0;
};


#endif