#ifndef _IRESPONSEBUILDER_H
#define _IRESPONSEBUILDER_H
#include <memory>
#include <pbnjson.hpp>
#include "UwbRangingInfo.h"

class IResponseBuilder {
public:
    virtual ~IResponseBuilder() = default;
    virtual void buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo) = 0;  
    
    
};


#endif