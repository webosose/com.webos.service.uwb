#ifndef _MODULEINFO_H
#define _MODULEINFO_H

#include <string>

class ModuleInfo {
public:
    static ModuleInfo& getInstance() {
        static ModuleInfo instance; 
        return instance;
    }
    
    void setFwVersion(const std::string& version);
    void setFwCrc(const std::string& crc);
    void setModuleState(const std::string& state);
    void setDeviceRole(const std::string& role);
    void setDeviceMode(const std::string& mode);
    void setPairingFlag(bool flag);
    void setDiscoveryStatus(bool discoveryStatus);
    void setUwbMacAddress(const std::string& address);
    void setDeviceName(const std::string& deviceName);
    void resetModuleInfo();
    
    std::string getFwVersion();
    std::string getFwCrc();
    std::string getModuleState();
    std::string getDeviceRole();
    std::string getDeviceMode();
    bool getPairingFlag();
    bool getDiscoveryStatus();
    std::string getUwbMacAddress();
    std::string getDeviceName();

private:
    ModuleInfo();
    ~ModuleInfo()= default;
    ModuleInfo(const ModuleInfo&)= delete;
    ModuleInfo& operator=(const ModuleInfo&)= delete;

    std::string mDeviceName = "";
    std::string mFwVersion = "";
    std::string mFwCrc = "";
    std::string mModuleState = "disconnect";
    std::string mDeviceRole = "";
    std::string mDeviceMode = "";
    bool mPairingFlag = false;
    std::string mUwbMacAddress = "";
    bool mDiscoveryStatus = false;
};

#endif