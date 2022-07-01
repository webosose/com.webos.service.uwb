#ifndef _UWBRESPONSEBUILDER_H
#define _UWBRESPONSEBUILDER_H

#include "IResponseBuilder.h"

class UwbResponseBuilder : public IResponseBuilder {
private:
    void buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo) override;
    void buildSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info) override;
    void buildServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable) override;
};


#endif
