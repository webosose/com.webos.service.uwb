#ifndef _UARTSERIAL_H
#define _UARTSERIAL_H

#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <cstring>
#include "CallbackInterface.h"
#include "UartConstants.h"
#include "UwbErrors.h"
#include "ModuleInfo.h"

#define NUM_PRINT_BYTES  16
enum {
    STRING = 1,
    BINARY,
};

class UartSerial {
public:
    void setAdaptor(std::shared_ptr<CallbackInterface> adapter);
    void InitializeUart(std::string param);
    UwbErrorCodes setUwbModuleState(CommandId cmdId);
    UwbErrorCodes getUwbModuleInfo();

private:
    uint32_t dataCount; //TODO: For testing. Can be removed.
    int mUartFd = -1;
    bool rxFlag = false;
    bool exitFlag = false;
    std::thread rxThreadId;
    std::shared_ptr<CallbackInterface> mUwbAdaptor;
    ModuleInfo& mModuleInfo = ModuleInfo::getInstance();
    
    void configureUart();
    speed_t setBaudrate(const int speed);
    void printData(char *rx_bin, int rx_length);
    void printBytes(int type, int length, const char *buffer);
    void serialDataRead();
    void processRangingInfo(char *rx_bin);
    void processDisconnectInfo(char *rx_bin);
    void processCommandResponse(char *rx_bin);
    void processModuleInfo(char *rx_bin);
};


#endif