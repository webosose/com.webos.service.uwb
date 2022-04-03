#include "UwbResponseBuilder.h"

void UwbResponseBuilder::buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo)
{
    pbnjson::JValue rangingInfoObj = pbnjson::Object();
    rangingInfoObj.put("remoteDeviceAddress", rangingInfo->getRemoteDevAddr());
    rangingInfoObj.put("condition", rangingInfo->getCondition()); 
    rangingInfoObj.put("connectionStatus", rangingInfo->getConnectionStatus());
    
    pbnjson::JValue receivedDataObj = pbnjson::Object();
    receivedDataObj.put("elapsedRealTime", rangingInfo->getElapsedTime());
    
    pbnjson::JValue distanceMeasureObj = pbnjson::Object();
    distanceMeasureObj.put("meter", rangingInfo->getDistanceMeasure()->getMeters());
    distanceMeasureObj.put("errorMeter", rangingInfo->getDistanceMeasure()->getErrorMeters());
    distanceMeasureObj.put("confidenceLevel", rangingInfo->getDistanceMeasure()->getConfidenceLevel());
    receivedDataObj.put("distanceMeasure", distanceMeasureObj);
    
    pbnjson::JValue aoaMeasureObj = pbnjson::Object();    
    pbnjson::JValue azimuthAngleMeasure = pbnjson::Object();
    azimuthAngleMeasure.put("radian", rangingInfo->getAzimuthAngleMeasure()->getRadians());
    azimuthAngleMeasure.put("errorRadian", rangingInfo->getAzimuthAngleMeasure()->getErrorRadians());
    azimuthAngleMeasure.put("confidenceLevel", rangingInfo->getAzimuthAngleMeasure()->getConfidenceLevel());
    aoaMeasureObj.put("azimuthAngleMeasure", azimuthAngleMeasure);
    
    pbnjson::JValue altitudeAngleMeasure = pbnjson::Object();
    altitudeAngleMeasure.put("radian", rangingInfo->getAltitudeAngleMeasure()->getRadians());
    altitudeAngleMeasure.put("errorRadian", rangingInfo->getAltitudeAngleMeasure()->getErrorRadians());
    altitudeAngleMeasure.put("confidenceLevel", rangingInfo->getAltitudeAngleMeasure()->getConfidenceLevel());
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