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

void ModuleInfo::setDiscoveryStatus(bool discoveryStatus) {
    mDiscoveryStatus = discoveryStatus;
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

bool ModuleInfo::getDiscoveryStatus() {
    return mDiscoveryStatus;
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
    mDiscoveryStatus = false;
    mUwbMacAddress = "";
}