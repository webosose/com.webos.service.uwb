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
 //   virtual void buildSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info) = 0;
 //   virtual void buildServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable) = 0;
};


#endif