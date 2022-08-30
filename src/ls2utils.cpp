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

#include <pbnjson.hpp>
#include <luna-service2/lunaservice.hpp>
#include "UwbLogging.h"
#include "ls2utils.h"

bool LSUtils::generatePayload(const pbnjson::JValue &object, std::string &payload)
{
    pbnjson::JGenerator serializer(NULL);
    return serializer.toString(object, pbnjson::JSchema::AllSchema(), payload);
}

void LSUtils::respondWithError(LS::Message &message, const std::string& errorText, unsigned int errorCode, bool failedSubscription)
{
    pbnjson::JValue responseObj = pbnjson::Object();

    if (failedSubscription)
        responseObj.put("subscribed", false);
    responseObj.put("returnValue", false);
    responseObj.put("errorText", errorText);
    responseObj.put("errorCode", (int) errorCode);

    std::string payload;
    LSUtils::generatePayload(responseObj, payload);

    message.respond(payload.c_str());
}

void LSUtils::postToSubscriptionPoint(std::shared_ptr<LS::SubscriptionPoint> subscriptionPoint, pbnjson::JValue &object)
{
    std::string payload;
    LSUtils::generatePayload(object, payload);

    subscriptionPoint->post(payload.c_str());
}


void LSUtils::postToClient(LS::Message &message, pbnjson::JValue &object)
{
    std::string payload;
    LSUtils::generatePayload(object, payload);

    try
    {
        message.respond(payload.c_str());
    }
    catch (LS::Error &error)
    {
        UWB_LOG_ERROR("postToClient : Failed to submit response [%s]", error.what());
    }
}

