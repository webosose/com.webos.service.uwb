#include <string>
#include <map>

#include "UwbErrors.h"

static std::map<UwbErrorCodes, std::string> uwbErrorTextTable =
{
	{UWB_ERROR_NONE, "No error"},
    {UWB_ERR_BAD_JSON, "Invalid JSON input"},
    {UWB_ERR_SCHEMA_VALIDATION_FAIL, "The JSON input does not match the expected schema"},
    {UWB_ERR_POWER_STATE_CHANGE_FAIL, "Failed to change UWB module state"},
    {UWB_ERR_NOT_VALID_INPUT, "Given input data is invalid"},
    {UWB_ERR_ADAPTER_NOT_AVAILABLE, "UWB adapter is not available"},
    {UWB_ERR_MTHD_NOT_SUBSCRIBED, "Method needs to be subscribed"},
    {UWB_ERR_DISCOVERY_TO_NEG_VALUE, "Invalid negative value for discoveryTimeout: "},
    {UWB_ERR_START_DISC_FAIL, "Failed to start discovery"},
    {UWB_SUBSCRIPTION_ERROR, "Method Subscription error"},
};

const std::string retrieveErrorText(UwbErrorCodes errorCode)
{
	return uwbErrorTextTable[errorCode];
}