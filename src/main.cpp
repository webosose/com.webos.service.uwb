#include <glib.h>
#include <string>
#include <memory>
#include <pbnjson.hpp>
#include "UwbLogging.h"
#include "UwbServiceManager.h"
#include "UwbServiceManager.cpp"
#include "UartSerial.h"
#include "UwbAdaptor.h"
#include "UartSerial.cpp"

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

    UWB_LOG_INFO("UwbService Main : start com.webos.service.uwb-1");
    if (mainLoop == NULL) {
        UWB_LOG_DEBUG("mainLoop not created");
        return EXIT_FAILURE;
    }

    UWB_LOG_INFO("UwbService Main : start com.webos.service.uwb-2");
    UwbServiceManager<UwbAdaptor> *uwbService = UwbServiceManager<UwbAdaptor>::getInstance();

    if (uwbService->init(mainLoop) == false) {
        UWB_LOG_INFO("UwbService Main : start com.webos.service.uwb-3");
        g_main_loop_unref(mainLoop);
        return EXIT_FAILURE;
    }

    //Start uart communication, To be modified in refactoring step
    auto uartSerial = std::make_shared<UartSerial<UwbAdaptor>>();
    std::thread uartThread(&UartSerial<UwbAdaptor>::InitializeUart, uartSerial, "Init Uart");
    
    //End of start uart communication

    UWB_LOG_INFO("UWB service started.");
    g_main_loop_run(mainLoop);
    UWB_LOG_INFO("UWB service stopped.");
    g_main_loop_unref(mainLoop);
    uwbService->deinit();

    return EXIT_SUCCESS;
}
