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
    UWB_ERR_MTHD_NOT_SUBSCRIBED = 106,
    UWB_ERR_DISCOVERY_TO_NEG_VALUE = 107,
    UWB_ERR_START_DISC_FAIL = 108,
    UWB_SUBSCRIPTION_ERROR = 109,
    UWB_UART_WRITE_FAILED = 110,
    UWB_ERROR_DEVICENAME_LENGTH = 111,
    UWB_ERROR_UNSUPPORTED_API_CONTROLEE = 112,
    UWB_ERROR_UNSUPPORTED_API_CONTROLLER = 113,
    UWB_ERROR_ADDRESS_LENGTH = 114,
    UWB_ERROR_DISCOVERY_IN_PROGRESS = 115,
    UWB_ERR_ADV_TIMEOUT_NEG_VALUE = 116,
    UWB_ERR_WRONG_DEV_STATE = 117,
};

const std::string retrieveErrorText(UwbErrorCodes errorCode);

#endif
