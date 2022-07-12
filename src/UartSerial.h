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
#include<sstream>
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
    void setEventListener(std::shared_ptr<CallbackInterface> eventListener);
    void InitializeUart(std::string param);
    UwbErrorCodes setUwbModuleState(CommandId cmdId);
    UwbErrorCodes getUwbModuleInfo();
    UwbErrorCodes setDeviceType(uint8_t deviceType);
    UwbErrorCodes setDeviceMode(uint8_t deviceMode);
    UwbErrorCodes getDeviceName();
    UwbErrorCodes setDeviceName(const std::string& deviceName);
    UwbErrorCodes getPairingInfo();
    UwbErrorCodes setScanTime(int32_t discoveryTimeout);
    UwbErrorCodes startDiscovery();
    UwbErrorCodes stopDiscovery();
    UwbErrorCodes openSession(const std::string& address);
    UwbErrorCodes closeSession(uint8_t sessionId);
    UwbErrorCodes startRanging(uint8_t sessionId);
    UwbErrorCodes stopRanging(uint8_t sessionId);

private:
    uint32_t dataCount = 0; //TODO: For testing. Can be removed.
    int mUartFd = -1;
    bool rxFlag = false;
    bool exitFlag = false;
    std::thread rxThreadId;
    uint8_t mDeviceType = 0x00;
    uint8_t mDeviceMode = 0x00;
    std::string mDeviceName = "";
    
    std::shared_ptr<CallbackInterface> mEventListener;
    ModuleInfo& mModuleInfo = ModuleInfo::getInstance();
    
    void configureUart();
    speed_t setBaudrate(const int speed);
    void printData(char *rx_bin, int rx_length);
    void printBytes(int type, int length, const char *buffer);
    void serialDataRead();
    void processCommonEvent(char *rx_bin);
    void processModuleInfo(char *rx_bin);
    void processDeviceName(char *rx_bin);
    void processPairingInfo(char *rx_bin);
    void processMeasurement(char *rx_bin);
    void processScanResult(char *rx_bin);
    bool IsUwbConnected();
};


#endif