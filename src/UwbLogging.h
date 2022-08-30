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

#ifndef UWBLOGGING_H
#define UWBLOGGING_H

#include <PmLogLib.h>

#define UWB_LOG_MSG_ID    "UWBService"

extern PmLogContext gUwbLogContext;

#define UWB_LOG_DEBUG(...) \
    PmLogDebug(gUwbLogContext, __VA_ARGS__)
#define UWB_LOG_INFO(...) \
    PmLogInfo(gUwbLogContext, UWB_LOG_MSG_ID, 0, __VA_ARGS__)
#define UWB_LOG_WARNING(...) \
    PmLogWarning(gUwbLogContext, UWB_LOG_MSG_ID, 0, __VA_ARGS__)
#define UWB_LOG_ERROR(...) \
    PmLogError(gUwbLogContext, UWB_LOG_MSG_ID, 0, __VA_ARGS__)
#define UWB_LOG_CRITICAL(...) \
    PmLogCritical(gUwbLogContext, UWB_LOG_MSG_ID, 0, __VA_ARGS__)


#endif

