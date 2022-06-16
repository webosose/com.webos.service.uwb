#include "UartSerial.h"

using namespace std;

void UartSerial::setAdaptor(shared_ptr<CallbackInterface> adapter) {
    dataCount = 0;
    mUwbAdaptor = adapter;
}

void UartSerial::InitializeUart(std::string param) {
    std::cout << param << std::endl;    
    configureUart();
    
    //create RX thread
    rxThreadId = std::thread(&UartSerial::serialDataRead, this);
    while (rxFlag != true) {
        usleep(10000);
    }
    getUwbModuleInfo();
    rxThreadId.join();

    close(mUartFd);
    return;
}

void UartSerial::configureUart() {
    struct termios options;
    std::cout << "UART Serial Communicator for LGE UWB Module" << std::endl;

    /* Open UART Device */
    do {
        //To be applied for using deviceName that is set by user configuration
        mUartFd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY /*| O_NDELAY*/);
        printf("uart open fd = %d \n", mUartFd);

        //fd control to be adjusted
        int ret_f = fcntl(mUartFd, F_SETFL, 0);
        if (mUartFd == -1) {
            printf("Error - Unable to open UART\n");
            //close(mUartFd);
            //return -1;
        }
    } while (mUartFd < 0);

    int speed = 115200; //TEMP coding
    speed_t baudRate = setBaudrate(speed);
    if (baudRate == -2) {
        printf("Error - Please do not use the speed!!! \n");
        return;
    }
    printf("UART Baud Rate is set as [%d] \n", speed);

    /* Set Config UART */
    tcgetattr(mUartFd, &options);
    options.c_cflag = baudRate | CS8 | CLOCAL | CREAD | IXOFF; //<Set baud rate
    options.c_iflag = IGNPAR | ICRNL;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_lflag &= ~(ECHO | ICANON | ONLCR);
    options.c_cc[VMIN] = 35; //Setup Minimum size of bytes (characters), LGE UWB UART protocol use the fixed size of uart serial data length (35)
    options.c_cc[VTIME] = 0;
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    //tcflush(mUartFd, TCIFLUSH);
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
    char rx_bin[35] = {0,};

    do {        
    //    char rx_hex[256] = {0,};
        int rx_length = 0;

        //Read up to 35 characters from the port if they are there
        rx_length = read(mUartFd, (void*)rx_bin, 35);

        printf("mUartFd=%d, rx_length=%d \n", mUartFd, rx_length);
        //To check the status of the mUartFd wheter it is available or not (rx_length <=0)

        if (rx_length <= 0) {
            printf("UART is not available!!! \n");
            return;
            //UART re-open & reconfigure logic SHOULD be added in here.
        }
        
        printData(rx_bin, rx_length);

        if (rx_length == 35) {
            //Bytes received
        //    rx_bin[rx_length] = '\0';
            //bin2hex(rx_bin, rx_length, rx_hex);            
        //    printData(rx_bin, rx_length);
            
            //Preamble Code == 0xcc
            if( rx_bin[0] == 0xcc ) {
                //Command Number
                EventId eventId = static_cast<EventId>(rx_bin[1]);
                switch(eventId) {
                    case HOST_EVT_UWB_MODULE_INFO :{
                        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n");
                        printf("HOST_EVT_UWB_MODULE_INFO (0x81) received. \n");
                    //    processModuleInfo(rx_bin);
                        break;
                    }
                    case HOST_STATUS_LOCATION_INFO : {
                        printf("HOST_STATUS_LOCATION_INFO (0x51) received. \n");
                        processRangingInfo(rx_bin);
                        break;
                    }
                    case HOST_STATUS_DISCONNECT : {
                        printf("-------------------------------------------------------------------------------------------------- \n");
                        printf("HOST_STATUS_DISCONNECT (0x62) received. \n");
                        processDisconnectInfo(rx_bin);
                        break;
                    }
                    case HOST_STATUS_MODULE_INFO : {
                        printf("HOST_STATUS_MODULE_INFO (0x3C) received. \n");
                        processModuleInfo(rx_bin);
                        break;
                    }
                    case HOST_STATUS_PAIRING_INFO : {
                        printf("HOST_STATUS_PAIRING_INFO (0x3D) received. \n");
                        break;
                    }
                    case HOST_EVT_MEASUREMENT : {
                        printf("HOST_EVT_MEASUREMENT (0x3E) received. \n");
                        break;
                    }
                    case HOST_EVT_MEASUREMENT_BYPASS : {
                        printf("HOST_EVT_MEASUREMENT_BYPASS (0x3F) received. \n");
                        break;
                    }
                    case HOST_EVT_TRANSFER_DATA : {
                        printf("HOST_EVT_TRANSFER_DATA (0x40) received. \n");
                        break;
                    }
                    case HOST_EVT_SCAN_RESULT : {
                        printf("HOST_EVT_SCAN_RESULT (0x41) received. \n");
                        break;
                    }
                    case HOST_EVT_DISCONNECT : {
                        printf("HOST_EVT_DISCONNECT (0x42) received. \n");
                        break;
                    }
                    case HOST_EVT_CONNECT : {
                        printf("HOST_EVT_CONNECT (0x43) received. \n");
                        break;
                    }
                    case HOST_EVT_CMD : {
                        printf("HOST_EVT_CMD (0x44) received. \n");
                        processCommandResponse(rx_bin);
                        break;
                    }
                    case HOST_EVT_DEVICE_NAME : {
                        printf("HOST_EVT_DEVICE_NAME (0x44) received. \n");
                        break;
                    }
                    case HOST_ACK_MODULE : {
                        printf("HOST_ACK_MODULE (0x45) received. \n");
                        break;
                    }
                    case HOST_ACK_SERVICE : {
                        printf("HOST_ACK_SERVICE (0x46) received. \n");
                        break;
                    }
                    case HOST_ACK : {
                        printf("*************************************************************************************************** \n");
                        printf("HOST_ACK (0xF0) received. \n");
                        break;
                    }
                    default : {
                        printf("##### Undefined Event Id = [%02x] ##### \n", eventId);
                    }
                }//switch(eventId)
            }
            else {
                printf("Invalid Preamble Code!!!\n");
            } //rx_bin[0] == 0xcc

            printf("\n");

        } else {
            printf("Invalid the length of Rx data!!! \n");
        } //rx_length == 35

        //TBC
        usleep(100000);
        memset(rx_bin, 0, 35);
    } while (exitFlag == false);

    return;
}

void UartSerial::processModuleInfo(char *rx_bin) {
    std::string fwVersion = to_string(rx_bin[2]) + "." + to_string(rx_bin[3]) + "." + to_string(rx_bin[4]);
    std::string fwCrc = to_string(rx_bin[5]) + " " + to_string(rx_bin[6]);
    
    std::string moduleState = "";
    if(rx_bin[7] == 0x00) {
        moduleState = "stopped";
    }
    else if(rx_bin[7] == 0x01) {
        moduleState = "active";
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

    cout << "fwVersion:" << fwVersion << endl;
    cout << "fwCrc:" << fwCrc << endl;
    cout << "moduleState:" << moduleState << endl;
    cout << "deviceRole:" << deviceRole << endl;
    cout << "deviceMode:" << deviceMode << endl;
    cout << "pairingFlag:" << pairingFlag << endl;
    cout << "uwbMacAddress:" << uwbMacAddress << endl;

    mModuleInfo.setModuleState(moduleState);
    mModuleInfo.setFwVersion(fwVersion);
    mModuleInfo.setFwCrc(fwCrc);
    mModuleInfo.setDeviceRole(deviceRole);
    mModuleInfo.setDeviceMode(deviceMode);
    mModuleInfo.setPairingFlag(pairingFlag);
    mModuleInfo.setUwbMacAddress(uwbMacAddress);
}

UwbErrorCodes UartSerial::getUwbModuleInfo() {
    std::vector<uint8_t> data (35, 0x00);
    data[0] = PREAMBLE;
    data[1] = 0x03;
    
    data[33] = 0x0d;
    data[34] = 0x0a;
    
    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    printf("numBytesWritten=%d \n", numBytesWritten);
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

void UartSerial::processCommandResponse(char *rx_bin) {
    CommandId commandId = static_cast<CommandId>(rx_bin[2]);
    uint8_t commandResult = rx_bin[3];
    printf("- commandResult: \t [%02x] \n", commandResult );
    switch(commandId) {
        case HOST_CMD_MODULE_START : {
            if(commandResult == 1) {
                mUwbAdaptor->updateModuleStateChanged("active");
            }
            else if(commandResult == 0) {
                //TODO: return error code
            }
            break;
        }
        case HOST_CMD_MODULE_STOP : {
            if(commandResult == 1) {
                mUwbAdaptor->updateModuleStateChanged("stopped");
            }
            else if(commandResult == 0) {
                //TODO: return error code
            }            
            break;
        }
        default : {
            printf("##### command Id = [%02x] ##### \n", commandId);
        }
    }
}

UwbErrorCodes UartSerial::setUwbModuleState(CommandId cmdId) {
    printf("cmdId=%d \n", cmdId);
    std::vector<uint8_t> data (35, 0x00);
    data[0] = PREAMBLE;
    data[1] = cmdId;
    
    data[33] = 0x0d;
    data[34] = 0x0a;
    ssize_t numBytesWritten = write(mUartFd, data.data(), data.size());
    printf("numBytesWritten=%d \n", numBytesWritten);
    if(numBytesWritten == -1) {
        return UWB_UART_WRITE_FAILED;
    }
    return UWB_ERROR_NONE;
}

void UartSerial::printData(char *rx_bin, int rx_length) {
    printf("UWB RX BUFFER (Length=%d) \n", rx_length);
    printf("Hexa String : ");
    printBytes(BINARY, rx_length, rx_bin);

    printf("Angle: [%d], Distance: [%d] \n", (int)(rx_bin[3]), (int)(rx_bin[5]) );
    printf("rx_bin int_val: [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] \n",
                (int)(rx_bin[0]), (int)(rx_bin[1]), (int)(rx_bin[2]), (int)(rx_bin[3]), (int)(rx_bin[4]), (int)(rx_bin[5]), (int)(rx_bin[6]), (int)(rx_bin[7]), (int)(rx_bin[8]));

    printf("- Preamble: \t Hexa[%02x] \t Int[%d] \n", rx_bin[0], (int)(rx_bin[0]) );
    printf("- Command: \t Hexa[%02x] \t Int[%d] \n", rx_bin[1], (int)(rx_bin[1]) );
    printf("- Device ID: \t Hexa[%02x] \t Int[%d] \n", rx_bin[2], (int)(rx_bin[2]) );
    printf("- Condition: \t Hexa[%02x] \t Int[%d] \n", rx_bin[7], (int)(rx_bin[7]) );
    printf("dataCount:%d\n", ++dataCount);
}

void UartSerial::processRangingInfo(char *rx_bin) {
    //Reading for Little Endian, TBD : we should use Big-Endian that is generic in network protocols
    short angle = rx_bin[3] | rx_bin[4] << 8;
    short dist = rx_bin[5] | rx_bin[6] << 8;
    //float dist_f = rx_bin[5] | rx_bin[6] << 8 | 0x00 << 16 | 0x00 << 24 ;
    //unsigned long dist_ul = (rx_bin[5])&0xFF | (rx_bin[6])&0xFF << 8 | 0x00&0xFF << 16 | 0x00&0xFF << 24 ;
    //float dist_f = *( (float*)&dist_ul );

    //Distance Calculation : 1 inch -> 2.54 cm
    //received distance == inch * 10
    //So, dist_cm = dist_inch (i.e., dist) * 2.54 / 10
    float dist_cm = dist * 2.54 / 10;
    printf("Device ID = [%02x %d] Angle[3~4] = [%d], Dist_inch*10[5~6] = [%d], Dist_cm = [%f], Condition = [%d] \n",
            rx_bin[2], (int)(rx_bin[2]), angle, dist, dist_cm, (int)(rx_bin[7]) );
    //Update UWB Data to UWB Adaptor
    mUwbAdaptor->updateRangingInfo((int)(rx_bin[7]), std::to_string( (int)(rx_bin[2]) ), (int)angle, (int)dist);
}

void UartSerial::processDisconnectInfo(char *rx_bin) {
    printf("Device ID = [%02x %d] \n", rx_bin[2], (int)(rx_bin[2]));
    mUwbAdaptor->updateDisconnectedDevice( (int)(rx_bin[2]) );
}

void UartSerial::printBytes(int type, int length, const char *buffer)
{
    int i;
    char temp[NUM_PRINT_BYTES] = {0,};

    for (i = 0; i < length; i++) {
        printf("%02x ", buffer[i]);
        temp[i%NUM_PRINT_BYTES] = buffer[i];
        if (((i + 1) % NUM_PRINT_BYTES) == 0) {
            if (type == STRING) {
                printf("\t%s", temp);
            }
            printf("\n");
            memset(temp, 0, NUM_PRINT_BYTES);
        }
    }
    if (type == STRING) {
        if (i % NUM_PRINT_BYTES != 0)
            printf("\t%s", temp);
    }
    printf("\n");
}


