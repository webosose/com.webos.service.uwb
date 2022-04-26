#include "UartSerial.h"

using namespace std;

void UartSerial::InitializeUart(std::string param) {
    std::cout << param << std::endl;    
    configureUart();
    
    //create RX thread
    rxThreadId = std::thread(&UartSerial::serialDataRead, this);
    while (rxFlag != true) {
        usleep(10000);
    }
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
        mUartFd = open("/dev/ttyUSB_LGEUWB", O_RDWR | O_NOCTTY | O_NDELAY);
        printf("uart open fd = %d \n", mUartFd);

        //fd control to be adjusted
        int ret_f = fcntl(mUartFd, F_SETFL, 0);
        if (mUartFd == -1) {
            printf("Error - Unable to open UART\n");
            //close(mUartFd);
            //return -1;
        }
    } while (mUartFd < 0);

    int speed = 9600; //TEMP coding
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
    options.c_cc[VMIN] = 22; //Setup Minimum size of bytes (characters), LGE UWB UART protocol use the fixed size of uart serial data length (22)
    options.c_cc[VTIME] = 0;
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    tcflush(mUartFd, TCIFLUSH);
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
    mUwbAdaptor = UwbAdaptor::getInstance();
    char rx_bin[128] = {0,};

    do {        
    //    char rx_hex[256] = {0,};
        int rx_length = 0;

        //Read up to 127 characters from the port if they are there
        rx_length = read(mUartFd, (void*)rx_bin, 128);

        printf("mUartFd=%d, rx_length=%d \n", mUartFd, rx_length);
        //To check the status of the mUartFd wheter it is available or not (rx_length <=0)

        if (rx_length <= 0) {
            printf("UART is not available!!! \n");
            return;
            //UART re-open & reconfigure logic SHOULD be added in here.
        }

        if (rx_length == 22) {
            //Bytes received
            rx_bin[rx_length] = '\0';
            //bin2hex(rx_bin, rx_length, rx_hex);            
            printData(rx_bin, rx_length);
            
            //Preamble Code == 0xcc
            if( rx_bin[0] == 0xcc ) {
            //Command Number
                switch(rx_bin[1]) {
                case 0x81 :{
                    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n");
                    printf("HOST_EVT_UWB_MODULE_INFO (0x81) received. \n");
                    processModuleInfo(rx_bin);                    
                    break;
                }
                case 0x51 : {
                    printf("HOST_STATUS_LOCATION_INFO (0x51) received. \n");                    
                    processRangingInfo(rx_bin);                 
                    break;
                }
                case 0x62 : {
                    printf("-------------------------------------------------------------------------------------------------- \n");
                    printf("HOST_STATUS_DISCONNECT (0x62) received. \n");
                    processDisconnectInfo(rx_bin);
                    break;
                }
                case 0xF0 : {
                    printf("*************************************************************************************************** \n");
                    printf("HOST_ACK (0xF0) received. \n");
                    break;
                }
                default : {
                    printf("##### Undefined Command Number = [%02x] ##### \n", rx_bin[1]);
                }
                }//switch(rx_bin[1])
            }
            else {
                printf("Invalid Preamble Code!!!\n");
            } //rx_bin[0] == 0xcc

            printf("\n");

        } else {
            printf("Invalid the length of Rx data!!! \n");
        } //rx_length == 22

        //TBC
        usleep(100000);
        memset(rx_bin, 0, 128);
    } while (exitFlag == false);

    return;
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
}

void UartSerial::processModuleInfo(char *rx_bin) {
    //Need to data definition doc for UWB Module State
    printf("UWB Module State : rx_bin[2] = [%02x %d] \n", rx_bin[2], (int)(rx_bin[2]) );
    //Need to data definition doc for FW Version
    printf("FW Version : rx_bin[3~5] = [%02x %02x %02x] \n", rx_bin[3], rx_bin[4], rx_bin[5]);
    //Need to data definition doc for FW CRC
    printf("FW CRC : rx_bin[6~9] = [%02x %02x %02x %02x] \n", rx_bin[6], rx_bin[7], rx_bin[8], rx_bin[9]);

    //Convert FW Version
    char str_fw_ver[4] = {0,};
    str_fw_ver[0] = rx_bin[3];
    str_fw_ver[1] = rx_bin[4];
    str_fw_ver[2] = rx_bin[5];
    str_fw_ver[3] = '\0';
    printf("FW Version - str_fw_ver : [%02x %02x %02x] \n",
            str_fw_ver[0], str_fw_ver[1], str_fw_ver[2]);
    printBytes(BINARY, 3, str_fw_ver);

    //Convert FW CRC
    char str_fw_crc[5] = {0,};
    str_fw_crc[0] = rx_bin[6];
    str_fw_crc[1] = rx_bin[7];
    str_fw_crc[2] = rx_bin[8];
    str_fw_crc[3] = rx_bin[9];
    str_fw_crc[4] = '\0';
    printf("FW CRC - str_fw_crc : [%02x %02x %02x %02x] \n",
            str_fw_crc[0], str_fw_crc[1], str_fw_crc[2], str_fw_crc[3]);
    //Update UWB Data to UWB Adaptor
    mUwbAdaptor->updateSpecificInfo((int)(rx_bin[2]), std::string("LGE UWB FW Version 1.0"), std::string("LGE UWB FW CRC temp"));   
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


