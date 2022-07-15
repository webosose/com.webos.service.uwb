#include "ModuleInfo.h"

ModuleInfo::ModuleInfo() {
    
}

void ModuleInfo::setFwVersion(const std::string& version) {
    mFwVersion = version;
}

void ModuleInfo::setFwCrc(const std::string& crc) {
    mFwCrc = crc;
}

void ModuleInfo::setModuleState(const std::string& state) {
    mModuleState = state;
}

void ModuleInfo::setDeviceRole(const std::string& role) {
    mDeviceRole = role;
}

void ModuleInfo::setDeviceMode(const std::string& mode) {
    mDeviceMode = mode;
}

void ModuleInfo::setPairingFlag(bool flag) {
    mPairingFlag = flag;
}

void ModuleInfo::setUwbMacAddress(const std::string& address) {
    mUwbMacAddress = address;
}

void ModuleInfo::setDeviceName(const std::string& deviceName) {
    mDeviceName = deviceName;
}

std::string ModuleInfo::getFwVersion(){
    return mFwVersion;
}

std::string ModuleInfo::getFwCrc(){
    return mFwCrc;
}

std::string ModuleInfo::getModuleState(){
    return mModuleState;
}

std::string ModuleInfo::getDeviceRole(){
    return mDeviceRole;
}

std::string ModuleInfo::getDeviceMode(){
    return mDeviceMode;
}

bool ModuleInfo::getPairingFlag(){
    return mPairingFlag;
}

std::string ModuleInfo::getUwbMacAddress(){
    return mUwbMacAddress;
}

std::string ModuleInfo::getDeviceName() { 
    return mDeviceName;
}

void ModuleInfo::resetModuleInfo() {
    mDeviceName = "";
    mFwVersion = "";
    mFwCrc = "";
    mModuleState = "disconnect";
    mDeviceRole = "";
    mDeviceMode = "";
    mPairingFlag = false;
    mUwbMacAddress = "";
}