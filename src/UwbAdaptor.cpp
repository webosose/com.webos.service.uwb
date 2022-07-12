#include "UwbAdaptor.h"
#include "UwbLogging.h"
#include "LunaUwbServiceUtil.h"

UwbAdaptor::UwbAdaptor() {
    UWB_LOG_INFO("UwbAdaptor Constructor");
}

UwbAdaptor::~UwbAdaptor() {

}

bool UwbAdaptor::init(LSHandle *sh) {
    UWB_LOG_INFO("UwbAdaptor Init");
    mLSHandle = sh;

    return true;
}

void UwbAdaptor::setDeviceInterface(std::shared_ptr<UartSerial> uartSerial) {
    mUartSerial = uartSerial;
}

UwbErrorCodes UwbAdaptor::startDiscovery(int32_t discoveryTimeout) {
    UWB_LOG_INFO("UwbAdaptor::startDiscovery");
    UwbErrorCodes error = UWB_ERROR_NONE;
    
    if(discoveryTimeout > 0) {
        error = mUartSerial->setScanTime(discoveryTimeout);
        if (error != UWB_ERROR_NONE) {
            return error;
        }
        usleep(50000);
    }
    error = mUartSerial->startDiscovery();
    return error;
}

bool UwbAdaptor::stopDiscovery(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::stopDiscovery");
    //TODO: Add call to driver API
    return true;
}

UwbErrorCodes UwbAdaptor::setUwbModuleState(const std::string& moduleState) {
    UWB_LOG_INFO("UwbAdaptor::setUwbModuleState");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(moduleState == "start") {
        error = mUartSerial->setUwbModuleState(HOST_CMD_MODULE_START);
    }
    else if(moduleState == "stop") {
        error = mUartSerial->setUwbModuleState(HOST_CMD_MODULE_STOP);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

UwbErrorCodes UwbAdaptor::getStatus() {
    UWB_LOG_INFO("UwbAdaptor::getStatus");

    UwbErrorCodes error = UWB_ERROR_NONE;
    error = mUartSerial->getUwbModuleInfo();
    return error;
}

bool UwbAdaptor::getPairedSessions(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::getPairedSessions");
    //TODO: Add call to driver API
    return true;
}

UwbErrorCodes UwbAdaptor::setDeviceType(const std::string& deviceType) {
    UWB_LOG_INFO("UwbAdaptor::setState");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(deviceType == "controller") {
        error = mUartSerial->setDeviceType(0);
    }
    else if(deviceType == "controlee") {
        error = mUartSerial->setDeviceType(1);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

UwbErrorCodes UwbAdaptor::setDeviceName(const std::string& deviceName) {
    UWB_LOG_INFO("UwbAdaptor::setDeviceName");
    UwbErrorCodes error = UWB_ERROR_NONE;
    error = mUartSerial->setDeviceName(deviceName);
    return error;
}

UwbErrorCodes UwbAdaptor::setDeviceMode(const std::string& deviceMode) {
    UWB_LOG_INFO("UwbAdaptor::setDeviceMode");
    UwbErrorCodes error = UWB_ERROR_NONE;
    if(deviceMode == "ranging") {
        error = mUartSerial->setDeviceMode(0);
    }
    else if(deviceMode == "dataTransfer") {
        error = mUartSerial->setDeviceMode(1);
    }
    else {
        error = UWB_ERR_NOT_VALID_INPUT;
    }

    return error;
}

bool UwbAdaptor::openSession(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::openSession");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::closeSession(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::closeSession");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::startRanging(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::startRanging");
    //TODO: Add call to driver API
    return true;
}

bool UwbAdaptor::stopRanging(LSMessage *message) {
    UWB_LOG_INFO("UwbAdaptor::stopRanging");
    //TODO: Add call to driver API
    return true;
}
