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

#include "LunaUwbServiceUtil.h"

typedef struct locErrorTextPair {
    UwbErrorCode code;
    const char *text;
} uwbErrorTextPair_t;

static const uwbErrorTextPair_t mapUwbErrorText[UWB_ERROR_MAX] = {
        {UWB_SUCCESS,                     "Success"},
        {UWB_UNKNOWN_ERROR,               "Unknown_Error"}
};
