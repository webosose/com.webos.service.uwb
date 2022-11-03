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

#ifndef _UWBSPECIFICINFO_H   //File need to be removed.
#define _UWBSPECIFICINFO_H
#include <string>

using namespace std;

class UwbSpecInfo {
public:
    static UwbSpecInfo& getInstance() {
        static UwbSpecInfo instance;
        return instance;
    }

    bool getModState() {
        return m_modState;
    }

    string getFwVersion() {
        return m_fwVersion;
    }

    string getFwCrc() {
        return m_fwCrc;
    }

    void setModState(bool mode) {
        m_modState = mode;
    }

    void setFwVersion(string version) {
        m_fwVersion = version;
    }

    void setFwCrc(string crc) {
        m_fwCrc = crc;
    }

private:
    UwbSpecInfo();
    UwbSpecInfo& operator=(const UwbSpecInfo&)= delete;
    bool m_modState = 0;
    string m_fwVersion = "0";
    string m_fwCrc = "0";
};
#endif
