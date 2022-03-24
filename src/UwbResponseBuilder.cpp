#include "UwbResponseBuilder.h"

void UwbResponseBuilder::buildRangingInfo(pbnjson::JValue &responseObj, std::unique_ptr<UwbRangingInfo>& rangingInfo)
{
    pbnjson::JValue rangingInfoObj = pbnjson::Object();
    rangingInfoObj.put("remoteDeviceAddress", rangingInfo->getRemoteDevAddr());
    rangingInfoObj.put("condition", rangingInfo->getCondition());
#if 0    
    int comp_connStatus = (int)(rangingInfo->getConnectionStatus());
    if( comp_connStatus == 0 ) {
        rangingInfoObj.put("connectionStatus", false);
    }
    else {
        rangingInfoObj.put("connectionStatus", true);
    }
#endif    
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
    azimuthAngleMeasure.put("radian", rangingInfo->getAngleMeasure()->getRadians());
    azimuthAngleMeasure.put("errorRadian", rangingInfo->getAngleMeasure()->getErrorRadians());
    azimuthAngleMeasure.put("confidenceLevel", rangingInfo->getAngleMeasure()->getConfidenceLevel());
    aoaMeasureObj.put("azimuthAngleMeasure", azimuthAngleMeasure);
    receivedDataObj.put("aoaMeasure", aoaMeasureObj);
    
    rangingInfoObj.put("receivedData", receivedDataObj);
    responseObj.put("rangingInfo", rangingInfoObj);
}