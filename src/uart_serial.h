#ifndef H_Uart_Serial
#define H_Uart_Serial

#include <stdio.h>

#include "UwbAdaptor.h"

void *uart_start(void *data);

// To update the data received from UWB Module (H/W) into UWB Adaptor in Service Core
static UwbAdaptor *mUwbAdaptor;

#endif //H_Uart_Serial
