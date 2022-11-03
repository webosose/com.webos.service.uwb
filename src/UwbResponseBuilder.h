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

#ifndef _UWBRESPONSEBUILDER_H
#define _UWBRESPONSEBUILDER_H

#include "IResponseBuilder.h"

class UwbResponseBuilder : public IResponseBuilder {
private:
    void buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo) override;
    void buildSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo mUwbSpecInfo) override;
};


#endif

