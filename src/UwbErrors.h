#ifndef UWB_ERRORS_H_
#define UWB_ERRORS_H_

#include <pbnjson.hpp>

enum UwbErrorCodes
{
    UWB_ERROR_NONE = 100,	
    UWB_ERR_BAD_JSON = 101,
    UWB_ERR_SCHEMA_VALIDATION_FAIL = 102,
    UWB_ERR_POWER_STATE_CHANGE_FAIL = 103,
    UWB_ERR_NOT_VALID_INPUT = 104,
    UWB_ERR_ADAPTER_NOT_AVAILABLE = 105,
};

const std::string retrieveErrorText(UwbErrorCodes errorCode);

#endif //UWB_ERRORS_H_
