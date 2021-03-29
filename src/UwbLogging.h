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


#endif // UWB_LOGGING_H

