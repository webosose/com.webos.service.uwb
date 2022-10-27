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

#include "UwbResponseBuilder.h"

void UwbResponseBuilder::buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo)
{
    pbnjson::JValue rangingInfoObj = pbnjson::Object();
    rangingInfoObj.put("remoteDeviceAddress", rangingInfo->getRemoteDevAddr());
    bool reliability = rangingInfo->getCondition() ? true : false;
    rangingInfoObj.put("reliability", reliability);
    rangingInfoObj.put("connectionStatus", rangingInfo->getConnectionStatus());

    pbnjson::JValue receivedDataObj = pbnjson::Object();
    receivedDataObj.put("elapsedRealTime", rangingInfo->getElapsedTime());

    pbnjson::JValue distanceMeasureObj = pbnjson::Object();
    distanceMeasureObj.put("distance", rangingInfo->getDistanceMeasure()->getMeters());
    receivedDataObj.put("distanceMeasure", distanceMeasureObj);

    pbnjson::JValue aoaMeasureObj = pbnjson::Object();    
    pbnjson::JValue azimuthAngleMeasure = pbnjson::Object();
    azimuthAngleMeasure.put("radian", rangingInfo->getAzimuthAngleMeasure()->getRadians());
    aoaMeasureObj.put("azimuthAngleMeasure", azimuthAngleMeasure);

    pbnjson::JValue altitudeAngleMeasure = pbnjson::Object();
    altitudeAngleMeasure.put("radian", rangingInfo->getAltitudeAngleMeasure()->getRadians());
    aoaMeasureObj.put("altitudeAngleMeasure", altitudeAngleMeasure);

    receivedDataObj.put("aoaMeasure", aoaMeasureObj);

    rangingInfoObj.put("receivedData", receivedDataObj);
    responseObj.put("rangingInfo", rangingInfoObj);
}

void UwbResponseBuilder::buildSpecificInfo(pbnjson::JValue &responseObj, UwbSpecInfo &info) {
    responseObj.put("modState",info.getModState());
    responseObj.put("fwVersion", info.getFwVersion());
    responseObj.put("fwCrc", info.getFwCrc());
}

void UwbResponseBuilder::buildServiceState(pbnjson::JValue &responseObj, bool isServiceAvailable) {
    responseObj.put("serviceAvailability", isServiceAvailable);
}
