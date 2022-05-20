#include <string>
#include <map>

#include "UwbErrors.h"

static std::map<UwbErrorCodes, std::string> uwbErrorTextTable =
{
	{UWB_ERR_ADAPTER_NOT_AVAILABLE, "No error"},
    {UWB_ERR_BAD_JSON, "Invalid JSON input"},
    {UWB_ERR_SCHEMA_VALIDATION_FAIL, "The JSON input does not match the expected schema"},
    {UWB_ERR_POWER_STATE_CHANGE_FAIL, "Failed to change UWB module state"},
    {UWB_ERR_NOT_VALID_INPUT, "Given input data is invalid"},
    {UWB_ERR_ADAPTER_NOT_AVAILABLE, "Bluetooth adapter is not available"},
};

const std::string retrieveErrorText(UwbErrorCodes errorCode)
{
	return uwbErrorTextTable[errorCode];
}