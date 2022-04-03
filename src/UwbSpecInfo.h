#ifndef _UWBSPECIFICINFO_H
#define _UWBSPECIFICINFO_H
#include <string>

using namespace std;

class UwbSpecInfo {
public:
    static UwbSpecInfo* getInstance();

    ~UwbSpecInfo();

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

    static UwbSpecInfo *m_uwbSpecInfo;
    bool m_modState;
    string m_fwVersion;
    string m_fwCrc;
};
#endif
