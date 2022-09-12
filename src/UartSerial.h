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
#include <sstream>
#include <fstream>
#include <map>
#include "CallbackInterface.h"
#include "UartConstants.h"
#include "UwbErrors.h"
#include "ModuleInfo.h"
#include "PairingInfo.h"

class UartSerial {
public:
    void setEventListener(std::shared_ptr<CallbackInterface> eventListener);
    void InitializeUart(std::string param);
    void processTime(int32_t time,uint8_t& lsb,uint8_t& msb);
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
    UwbErrorCodes openSessionControlee(int32_t advTimeout);
    UwbErrorCodes closeSession(uint8_t sessionId);
    UwbErrorCodes startRanging(uint8_t sessionId);
    UwbErrorCodes stopRanging(uint8_t sessionId);

private:
    int mUartFd = -1;
    bool rxFlag = false;
    bool exitFlag = false;
    std::thread rxThreadId;
    uint8_t mDeviceType = 0x00;
    uint8_t mDeviceMode = 0x00;
    std::string mDeviceName = "";

    std::map<std::string,std::string> mdeviceMap{};
    std::shared_ptr<CallbackInterface> mEventListener;
    ModuleInfo& mModuleInfo = ModuleInfo::getInstance();
    PairingInfo& mPairingInfo = PairingInfo::getInstance();

    void configureUart();
    speed_t setBaudrate(const int speed);
    void printData(char *rx_bin, int rx_length);
    void printBytes(int length, const char *buffer);
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
