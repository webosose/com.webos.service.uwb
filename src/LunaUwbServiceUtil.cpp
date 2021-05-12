#include "LunaUwbServiceUtil.h"

typedef struct locErrorTextPair {
    UwbErrorCode code;
    const char *text;
} uwbErrorTextPair_t;

static const uwbErrorTextPair_t mapUwbErrorText[UWB_ERROR_MAX] = {
        {UWB_SUCCESS,                     "Success"},
        {UWB_UNKNOWN_ERROR,               "Unknown_Error"}
};
