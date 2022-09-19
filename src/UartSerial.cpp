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

#include "UartSerial.h"
#include "UwbLogging.h"

using namespace std;

void UartSerial::setEventListener(shared_ptr<CallbackInterface> eventListener) {
    mEventListener = eventListener;
}

bool UartSerial::IsUwbConnected()
{
    bool connected = false;
    ifstream fileHandler;

    fileHandler.open("/dev/ttyUSB_LGEUWB");

    if(fileHandler.is_open())
    {
        connected = true;
        UWB_LOG_INFO("UWB module connected!!!");
    }
    else{
        UWB_LOG_ERROR("UWB module not connected!!!");
    }
    fileHandler.close();
    return connected;
}

void UartSerial::InitializeUart(std::string param) {
    UWB_LOG_DEBUG("InitializeUart: %s", param.c_str());
    bool connected = false;
    do {
        connected = IsUwbConnected();
        sleep(1);
    }while(connected == false);

    configureUart();

    //create RX thread
    rxThreadId = std::thread(&UartSerial::serialDataRead, this);
    while (rxFlag != true) {
        usleep(10000);
    }
    getDeviceName();
    usleep(100000);
    getUwbModuleInfo();
    usleep(100000);
    getPairingInfo();
    rxThreadId.join();
    close(mUartFd);

    sleep(1);
    rxFlag = false;
    mModuleInfo.resetModuleInfo();
    mEventListener->updateModuleStatus();

    InitializeUart("UWB Device not Available");
    return;
}

void UartSerial::configureUart() {
    struct termios options;
    UWB_LOG_DEBUG("starting Serial port configuration for UWB module");

    /* Open UART Device */
    do {
        //To be applied for using deviceName that is set by user configuration
        mUartFd = open("/dev/ttyUSB_LGEUWB", O_RDWR | O_NOCTTY /*| O_NDELAY*/);
        UWB_LOG_DEBUG("uart open fd = %d", mUartFd );

        //TODO:fd control to be adjusted
        if (mUartFd == -1) {
            UWB_LOG_ERROR("Error - Unable to open UART");
            usleep(100000); //sleep before retry
        }
    } while (mUartFd < 0);

    int speed = 115200; //TODO:TEMP coding
    speed_t baudRate = setBaudrate(speed);
    if (baudRate == -2) {
        UWB_LOG_ERROR("Error: Please do not use the baud rate!!!");
        return;
    }
    UWB_LOG_DEBUG("UART Baud Rate is set as [%d]", speed);

    /* Set Config UART */
    tcgetattr(mUartFd, &options);
    options.c_cflag = baudRate | CS8 | CLOCAL | CREAD | IXOFF; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_lflag &= ~(ECHO | ICANON | ONLCR);
    options.c_cc[VMIN] = UART_DATA_SIZE; //Setup Minimum size of bytes (characters), LGE UWB UART protocol use the fixed size of uart serial data length (UART_DATA_SIZE)
    options.c_cc[VTIME] = 0;
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    tcflush(mUartFd, TCIOFLUSH);
    tcsetattr(mUartFd, TCSANOW, &options);
}

speed_t UartSerial::setBaudrate(const int speed)
{
    speed_t myBaud;

    switch (speed)
    {
        case      50:   myBaud =      B50 ; break ;
        case      75:   myBaud =      B75 ; break ;
        case     110:   myBaud =     B110 ; break ;
        case     134:   myBaud =     B134 ; break ;
        case     150:   myBaud =     B150 ; break ;
        case     200:   myBaud =     B200 ; break ;
        case     300:   myBaud =     B300 ; break ;
        case     600:   myBaud =     B600 ; break ;
        case    1200:   myBaud =    B1200 ; break ;
        case    1800:   myBaud =    B1800 ; break ;
        case    2400:   myBaud =    B2400 ; break ;
        case    4800:   myBaud =    B4800 ; break ;
        case    9600:   myBaud =    B9600 ; break ;
        case   19200:   myBaud =   B19200 ; break ;
        case   38400:   myBaud =   B38400 ; break ;
        case   57600:   myBaud =   B57600 ; break ;
        case  115200:   myBaud =  B115200 ; break ;
        case  230400:   myBaud =  B230400 ; break ;
        case  460800:   myBaud =  B460800 ; break ;
        case  500000:   myBaud =  B500000 ; break ;
        case  576000:   myBaud =  B576000 ; break ;
        case  921600:   myBaud =  B921600 ; break ;
        case 1000000:   myBaud = B1000000 ; break ;
        case 1152000:   myBaud = B1152000 ; break ;
        case 1500000:   myBaud = B1500000 ; break ;
        case 2000000:   myBaud = B2000000 ; break ;
        case 2500000:   myBaud = B2500000 ; break ;
        case 3000000:   myBaud = B3000000 ; break ;
        case 3500000:   myBaud = B3500000 ; break ;
        case 4000000:   myBaud = B4000000 ; break ;

        default:
            return -2 ;
    }

    return myBaud;
}

void UartSerial::serialDataRead() {
    rxFlag = true;
    char rx_bin[UART_DATA_SIZE] = {0};

    do {
        int rx_length = 0;
        //Read up to 35 characters from the port if they are there
        rx_length = read(mUartFd, (void*)rx_bin, UART_DATA_SIZE);

        UWB_LOG_DEBUG("mUartFd=%d, rx_length=%d", mUartFd, rx_length);
        //To check the status of the mUartFd wheter it is available or not (rx_length <=0)

        if (rx_length <= 0) {
            UWB_LOG_ERROR("UART is not available!!!");
            return;
            //TODO:UART re-open & reconfigure logic SHOULD be added in here.
        }

        if (rx_length == UART_DATA_SIZE) {
             printData(rx_bin, rx_length);
            if( rx_bin[0] == PREAMBLE ) {
                EventId eventId = static_cast<EventId>(rx_bin[1]);
                UWB_LOG_INFO("Event Id = [%02x]",eventId);
                switch(eventId) {
                    case HOST_STATUS_MODULE_INFO : {
                        processModuleInfo(rx_bin);
                        break;
                    }
                    case HOST_STATUS_PAIRING_INFO : {
                        processPairingInfo(rx_bin);
                        break;
                    }
                    case HOST_EVT_MEASUREMENT : {
                        processMeasurement(rx_bin);
                        break;
                    }
                    case HOST_EVT_MEASUREMENT_BYPASS : {
                        break;
                    }
                    case HOST_EVT_TRANSFER_DATA : {
                        break;
                    }
                    case HOST_EVT_SCAN_RESULT : {
                        processScanResult(rx_bin);
                        break;
                    }
                    case HOST_EVT_DISCONNECT : {
                        break;
                    }
                    case HOST_EVT_CONNECT : {
                        break;
                    }
                    case HOST_EVT_CMD : {
                        processCommonEvent(rx_bin);
                        break;
                    }
                    case HOST_EVT_DEVICE_NAME : {
                        processDeviceName(rx_bin);
                        break;
                    }
                    case HOST_ACK_MODULE : {
                        break;
                    }
                    case HOST_ACK_SERVICE : {
                        break;
                    }
                    default : {
                        break;
                    }
                }
            }
            else {
                UWB_LOG_INFO("Invalid Preamble Code!!!");
            }

        } else {
            UWB_LOG_INFO("Invalid the length of Rx data!!!");
        }

        //TBC
        usleep(100000);
        memset(rx_bin, 0, UART_DATA_SIZE);
    } while (exitFlag == false);

    return;
}

UwbErrorCodes UartSerial::setUwbModuleState(CommandId cmdId) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = cmdId;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;
    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::getUwbModuleInfo() {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_MODULE_INFO;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

void UartSerial::processModuleInfo(char *rx_bin) {
    std::string fwVersion = to_string(rx_bin[4]) + "." + to_string(rx_bin[3]) + "." + to_string(rx_bin[2]);

    char fwCrc[10];
    sprintf(fwCrc, "%2X %2X", rx_bin[5], rx_bin[6]);

    std::string moduleState = "";
    if(rx_bin[7] == 0x00) {
        moduleState = "stop";
    }
    else if(rx_bin[7] == 0x01) {
        moduleState = "start";
    }
    else {
        moduleState = "Unknown";
    }

    std::string deviceRole = "";
    if(rx_bin[8] == 0x00) {
        deviceRole = "controller";
    }
    else if(rx_bin[8] == 0x01) {
        deviceRole = "controlee";
    }

    std::string deviceMode = "";
    if(rx_bin[9] == 0x00) {
        deviceMode = "ranging";
    }
    else if(rx_bin[9] == 0x01) {
        deviceMode = "dataTransfer";
    }

    bool pairingFlag = false;
    if(rx_bin[10] == 0x00) {
        pairingFlag = false;
    }
    else if(rx_bin[10] == 0x01) {
        pairingFlag = true;
    }

    char address[(8 * 3) + 1];
    char *ptr = &address[0];
    for (int i = 11; i < 19; i++) {
        ptr += sprintf(ptr, "%02X", rx_bin[i]);
        ptr += sprintf(ptr, ":");
    }
    std::string uwbMacAddress = address;
    uwbMacAddress.pop_back();

    mModuleInfo.setModuleState(moduleState);
    mModuleInfo.setFwVersion(fwVersion);
    mModuleInfo.setFwCrc(fwCrc);
    mModuleInfo.setDeviceRole(deviceRole);
    mModuleInfo.setDeviceMode(deviceMode);
    mModuleInfo.setPairingFlag(pairingFlag);
    mModuleInfo.setUwbMacAddress(uwbMacAddress);
    mEventListener->updateModuleStatus();
}

UwbErrorCodes UartSerial::getPairingInfo() {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_PAIRING_INFO;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

void UartSerial::processPairingInfo(char *rx_bin) {
    uint8_t pairingCount = rx_bin[2];
    mPairingInfo.setPairingCount(pairingCount);
    pbnjson::JValue pairingArray = pbnjson::Array();

    for(int i=2;i<(pairingCount*3);i += 3){
        uint8_t sessionId = rx_bin[i+1];

    std::string deviceRole = "";
    std::string deviceName = "";
    if(rx_bin[i+2] == 0x00){
        deviceRole = "Controller";
    }
    else{
        deviceRole = "Controlee";
    }
    bool connectionStatus = false;
    if(rx_bin[i+3] == 0x01)
        connectionStatus = true;

    pbnjson::JValue pairingObj = pbnjson::Object();
    pairingObj.put("sessionId", sessionId);
    pairingObj.put("deviceRole", deviceRole);
    pairingObj.put("deviceName", mPairingInfo.getPairedDeviceName(mMacAddress));
    pairingObj.put("connectionStatus", connectionStatus);
    pairingArray.append(pairingObj);
    }
    mPairingInfo.setSessionInfo(pairingArray);
    mEventListener->updatePairingInfo();
}

UwbErrorCodes UartSerial::setDeviceType(uint8_t deviceType) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_SET_DEVICE_TYPE;
    data[2] = deviceType;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    mDeviceType = deviceType;
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::setDeviceMode(uint8_t deviceMode) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_SET_DEVICE_MODE;
    data[2] = deviceMode;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    mDeviceMode = deviceMode;
    return UWB_ERROR_NONE;
}

void UartSerial::processTime(int32_t time,uint8_t& lsb,uint8_t& msb)
{
    lsb = time & 255;
    msb = ((time & (255 << 8)) >> 8);
}

UwbErrorCodes UartSerial::setScanTime(int32_t discoveryTimeout) {
    uint8_t lsb, msb = 0;
    int32_t timeout = discoveryTimeout*1000;

    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_SET_SCAN_TIME;
    processTime(timeout, lsb, msb);
    data[2] = lsb;
    data[3] = msb;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());;
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::startDiscovery() {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_DEVICE_DISCOVERY;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());;
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    mEventListener->updateDiscoveryStatus(true);
    return UWB_ERROR_NONE;
}

void UartSerial::processScanResult(char *rx_bin) {
    char address[(6*3)+1];
    char *ptr = &address[0];
    for (int i=2;i<8;i++) {
        ptr += sprintf(ptr, "%02X", rx_bin[i]);
        ptr += sprintf(ptr,":");
    }
    std::string macAddress = address;
    macAddress.pop_back();

    ostringstream name;
    for(int i=8;i<33;i++) {
        if(rx_bin[i] != '\0')
            name << rx_bin[i];
    }
    std::string deviceName  = name.str();

    int count = 0;
    static int count1 = 0;
    if(count1 == 10){
        mdeviceMap.clear();
        count1 = 0;
    }

    if(mdeviceMap.count(macAddress) == 0){
        mPairingInfo.setDiscoveredDevices(macAddress, deviceName);
        mdeviceMap.emplace(std::pair<std::string , std::string>(macAddress, deviceName));
        count ++;
    }
    if(count){
        mEventListener->updateScanResult(macAddress, deviceName);
    }
    count1++;
}

UwbErrorCodes UartSerial::stopDiscovery() {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_DISCOVERY_STOP;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());;
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::openSession(const std::string& address) {
    if(address.size()>17){
        return UWB_ERROR_ADDRESS_LENGTH;
    }
    mMacAddress = address;
    char *ptr = &address[0];
    char delim[] = ":";
    char *token = strtok(ptr,delim);

    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_PAIRING_REQUEST;
    int i = 0;
    while (token){
        sscanf(token, "%02x", &data[i+2]);
        i++;
        token = strtok(NULL,delim);
    }
    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }

    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::openSessionControlee(int32_t advTimeout) {
    uint8_t lsb, msb = 0;
    int32_t timeout = advTimeout*1000;

    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_ADVERTISING;
    processTime(timeout, lsb, msb);
    data[2] = lsb;
    data[3] = msb;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());;
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE; 
}

UwbErrorCodes UartSerial::closeSession(uint8_t sessionId) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_PAIRING_CANCEL;
    data[2] = sessionId;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::startRanging(uint8_t sessionId) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_START_RANGING_SESSION;
    data[2] = sessionId;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }

    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::stopRanging(uint8_t sessionId) {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_STOP_RANGING_SESSION;
    data[2] = sessionId;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }

    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::setDeviceName(const std::string& deviceName) {
    if(deviceName.size() > DEVICENAME_LENGTH){
        return UWB_ERROR_DEVICENAME_LENGTH;
    }

    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_SET_DEVICE_NAME;

    for (size_t i = 0; i < deviceName.size(); ++i)
    {
       data[i+2] = deviceName[i];
    }

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    mDeviceName = deviceName;
    return UWB_ERROR_NONE;
}

UwbErrorCodes UartSerial::getDeviceName() {
    std::vector<uint8_t> data (UART_DATA_SIZE, 0x00);
    data[0] = PREAMBLE;
    data[1] = HOST_REQ_DEVICE_NAME;

    data[UART_DATA_SIZE-2] = CRC_LSB;
    data[UART_DATA_SIZE-1] = CRC_MSB;

    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

void UartSerial::processDeviceName(char *rx_bin) {
    ostringstream os;
    for(int i=3;i<28;i++) {
        if(rx_bin[i] != '\0')
            os << rx_bin[i];
    }
    std::string deviceName  = os.str();
    mDeviceName = deviceName;
    mModuleInfo.setDeviceName(deviceName);
}


void UartSerial::processCommonEvent(char *rx_bin) {
    CommandId commandId = static_cast<CommandId>(rx_bin[2]);
    uint8_t commandResult = rx_bin[3];
    UWB_LOG_INFO("commandId: [%02x]", commandId);
    UWB_LOG_INFO("commandResult: [%02x]", commandResult);
    switch(commandId) {
        case HOST_CMD_MODULE_START : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("Module state changed");
                mEventListener->updateModuleStateChanged("start");
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_CMD_MODULE_STOP : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("Module state changed");
                mEventListener->updateModuleStateChanged("stop");
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_SET_DEVICE_TYPE : {
            if(commandResult == CMD_SUCCESS) {
                mEventListener->updateDeviceTypeChanged(mDeviceType);
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_SET_DEVICE_MODE : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("Device mode changed");
                mEventListener->updateDeviceModeChanged(mDeviceMode);
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_SET_DEVICE_NAME : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("Device name changed");
                mEventListener->updateDeviceNameChanged(mDeviceName);
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_SET_SCAN_TIME : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("Discovery time-out is set");
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_DISCOVERY_STOP : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("HOST_REQ_DISCOVERY_STOP successful");
                mEventListener->updateDiscoveryStatus(false);
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_PAIRING_REQUEST : {
            if(commandResult == CMD_SUCCESS) {
                uint8_t sessionId = rx_bin[4];
                UWB_LOG_INFO("sessionId: %d", sessionId );
                mEventListener->updateOpenSessionResponse(sessionId);
                mEventListener->updateDiscoveryStatus(false);
                mEventListener->updatePairingFlag(true);
                getPairingInfo();
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_ADVERTISING : {
            if(commandResult == CMD_SUCCESS) {
                UWB_LOG_INFO("HOST_REQ_ADVERTISING successful");
                mEventListener->updatePairingFlag(true);
                getPairingInfo();
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_PAIRING_CANCEL : {
            if(commandResult == CMD_SUCCESS) {
                uint8_t sessionId = rx_bin[4];
                UWB_LOG_INFO("sessionId: %d", sessionId );
                mEventListener->updatePairingFlag(false);
                getPairingInfo();
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_START_RANGING_SESSION : {
            if(commandResult == CMD_SUCCESS) {
                uint8_t sessionId = rx_bin[4];
                UWB_LOG_INFO("sessionId: %d", sessionId );
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        case HOST_REQ_STOP_RANGING_SESSION : {
            if(commandResult == CMD_SUCCESS) {
                uint8_t sessionId = rx_bin[4];
                UWB_LOG_INFO("sessionId: %d", sessionId );
            }
            else if(commandResult == CMD_FAILURE) {
                UWB_LOG_ERROR("Uart Command failed");
            }
            break;
        }
        default : {
            UWB_LOG_INFO("Unused commandId: [%02x]", commandId);
        }
    }
}

void UartSerial::processMeasurement(char *rx_bin) {
    uint8_t sessionId = rx_bin[2];
    UWB_LOG_DEBUG("sessionId: [%02x]", sessionId);
    int16_t angle = rx_bin[3] | rx_bin[4] << 8;
    int16_t distance = rx_bin[5] | rx_bin[6] << 8;
    uint8_t reliability = rx_bin[7];

    UWB_LOG_DEBUG("Angle = [%d], Dist_cm = [%d], reliability = [%d]",angle, distance, reliability);
    mEventListener->updateRangingInfo(reliability, sessionId, angle, distance);
}

void UartSerial::printData(char *rx_bin, int rx_length) {
    UWB_LOG_DEBUG("UWB RX BUFFER (Length=%d)", rx_length);
    printBytes(rx_length, rx_bin);
}

void UartSerial::printBytes(int length, const char *buffer)
{
    int i;
    char data[(length*3)+1];
    char *ptr = data;

    for (i = 0; i < length; i++) {
        ptr += sprintf(ptr, "%02X", buffer[i]);
        ptr += sprintf(ptr, " ");
    }
    UWB_LOG_DEBUG("%s", data);
}
