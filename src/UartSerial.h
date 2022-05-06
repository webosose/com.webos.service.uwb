#ifndef _UARTSERIAL_H
#define _UARTSERIAL_H

#include <string>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#define NUM_PRINT_BYTES  16
enum {
    STRING = 1,
    BINARY,
};

template <class T>
class UartSerial {
public:
    void InitializeUart(std::string param);

private:
    int mUartFd = -1;
    bool rxFlag = false;
    bool exitFlag = false;
    std::thread rxThreadId;
    T& mUwbAdaptor = T::getInstance();
    
    void configureUart();
    speed_t setBaudrate(const int speed);
    void printData(char *rx_bin, int rx_length);
    void printBytes(int type, int length, const char *buffer);
    void serialDataRead();
    void processModuleInfo(char *rx_bin);
    void processRangingInfo(char *rx_bin);
    void processDisconnectInfo(char *rx_bin);
};


#endif