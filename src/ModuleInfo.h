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
    void setUwbMacAddress(const std::string& address);
    
    std::string getFwVersion();
    std::string getFwCrc();
    std::string getModuleState();
    std::string getDeviceRole();
    std::string getDeviceMode();
    bool getPairingFlag();
    std::string getUwbMacAddress();

private:
    ModuleInfo();
    ~ModuleInfo()= default;
    ModuleInfo(const ModuleInfo&)= delete;
    ModuleInfo& operator=(const ModuleInfo&)= delete;


    std::string mFwVersion = "";
    std::string mFwCrc = "";
    std::string mModuleState = "";
    std::string mDeviceRole = "";
    std::string mDeviceMode = "";
    bool mPairingFlag = false;
    std::string mUwbMacAddress = "";
};

#endif