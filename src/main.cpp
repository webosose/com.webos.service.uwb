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

#include <glib.h>
#include <string>
#include <memory>
#include <pbnjson.hpp>
#include "UwbLogging.h"
#include "UwbServiceManager.h"
#include "UartSerial.h"
#include "UwbAdapterInterface.h"
#include "UwbAdaptor.h"
#include "UwbEventListener.h"

PmLogContext gUwbLogContext;
static const char* const logContextName = "webos-uwb-service";


int main(int argc, char *argv[]) {

    PmLogErr status = PmLogGetContext(logContextName, &gUwbLogContext);
    if (status != kPmLogErr_None)
    {
        fprintf(stderr, "Failed to set PmLog context %s\n", logContextName);
        abort();
    }


    UWB_LOG_INFO("UwbService Main : start com.webos.service.uwb");

    GMainLoop* mainLoop = g_main_loop_new(nullptr, false);
    if (mainLoop == NULL) {
        UWB_LOG_DEBUG("mainLoop not created");
        return EXIT_FAILURE;
    }

    UwbServiceManager *uwbService = UwbServiceManager::getInstance();
    auto adapter = std::make_shared<UwbAdaptor>();

    if (uwbService->init(mainLoop, adapter) == false) {
        g_main_loop_unref(mainLoop);
        return EXIT_FAILURE;
    }

    //Start uart communication, To be modified in refactoring step
    auto uartSerial = std::make_shared<UartSerial>();
    adapter->setDeviceInterface(uartSerial);
    auto eventListener = std::make_shared<UwbEventListener>();
    uartSerial->setEventListener(eventListener);
    std::thread uartThread(&UartSerial::InitializeUart, uartSerial, "Init Uart");

    //End of start uart communication

    UWB_LOG_DEBUG("UWB service started.");
    g_main_loop_run(mainLoop);
    UWB_LOG_DEBUG("UWB service stopped.");
    g_main_loop_unref(mainLoop);
    uwbService->deinit();

    return EXIT_SUCCESS;
}
